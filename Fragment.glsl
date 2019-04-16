#version 440
// these values are interpolated at the rasteriser
in vec3 color;
in vec3 fragPos;
in vec2 textureCoord;
in vec3 normal;
in vec4 shadow_coord;

// this is the final pixel colour
out vec4 fragment_color;

// this is a uniform value, the very same value for ALL pixel shader executions
layout(location = 5) uniform vec3 colourFromImGui;
layout(location = 16) uniform vec3 camPos;

// Texture inputs 
uniform sampler2D shadowMap;
uniform sampler2D diffuseTex;

//~~ LightCalc variables and structs.
struct PointLight
{
	vec3 pos;

	float constant;
	float linear;
	float quadratic;
	float range;
	float power;

	vec3 diffuse;
	vec3 specular;
};

struct DirectionalLight
{
	vec3 pos;
	vec3 dir;

	vec3 col;
	vec3 specular;
};

#define NR_P_LIGHTS 6
uniform PointLight pointLights[NR_P_LIGHTS];
uniform DirectionalLight dirLight;

float shadowCalc(vec4 shadow_coord, vec3 normal, vec3 light_pos);
vec3 CalculatePointLight(PointLight light, vec3 pixelPos, vec3 aNormal, vec3 viewDir);
vec3 CalculateDirLight(DirectionalLight light, vec3 aNormal, vec3 viewDir);

void main () {
	vec4 diffuse = texture(diffuseTex, vec2(textureCoord.s, 1 - textureCoord.t));

	vec3 norm = normalize(normal);
	vec3 viewDirection = normalize(camPos - fragPos);

	//float shadow = shadowCalc(shadow_coord, normal, dirLight.pos);
	vec3 ambientLight = diffuse.xyz * vec3(0.1f, 0.1f, 0.1f);
	vec3 directionalLight = vec3(0.0f);

	vec3 projLightCoords = shadow_coord.xyz / shadow_coord.w;
	projLightCoords = projLightCoords * 0.5 + 0.5;
	float shadowMapDepth = texture(shadowMap, projLightCoords.xy).r;
	float lightDepthValue = projLightCoords.z;
	float bias = 0.001f;
	//bias = max(0.05 * (1.0 - dot(normal, dirLight.dir)), 0.005);
	lightDepthValue -= bias;

	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projLightCoords.xy + vec2(x, y) * texelSize).r; 
            directionalLight += diffuse.xyz * vec3(lightDepthValue < pcfDepth ? 0.3f : 0.0f);        
        }    
    }
	directionalLight /= 9;

	if (projLightCoords.z > 1.0f)
	{
		directionalLight = diffuse.xyz * CalculateDirLight(dirLight, norm, viewDirection);
	}

	vec3 pointLight = vec3(0.0f);
	for(int i = 0; i < NR_P_LIGHTS; i++)
	{
		pointLight += diffuse.xyz * CalculatePointLight(pointLights[i], fragPos, norm, viewDirection);
	}

	vec4 finalColor = vec4(max(ambientLight + directionalLight + pointLight, 0.0f), 1.0f);
	fragment_color = finalColor;
}

/*
=============================================================
Sample from the depth map for shadows. 
=============================================================
*/
float shadowCalc(vec4 shadow_coord, vec3 normal, vec3 light_pos){
	
	vec3 proj_coord = shadow_coord.xyz / shadow_coord.w;
	proj_coord = proj_coord * 0.5 + 0.5;
	float closetsDepth = texture(shadowMap, proj_coord.xy).r;
	float currentDepth = proj_coord.z;
	vec3 lightDir = normalize(light_pos - fragPos.xyz);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float shadow = 0.0;

    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, proj_coord.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

	if(proj_coord.z > 1.0)
		shadow = 0.0;

	return shadow;
}


/*
=============================================================
Function for all the light calculations for a point light.

First we calculate the direction from the light to the current point.
Then the distance. We then check if that distance is less than the pointlight's range.
If it is less no calculations are made and we return diffuse specular and ambient empty.
If not we calculate diffuse, specular and attenuation and then add it all together.

TODO: When we add the material class its shininess, diffuse and specular should be added.

=============================================================
*/
vec3 CalculatePointLight(PointLight pLight, vec3 pixelPos, vec3 aNormal, vec3 viewDir)
{
	vec3 pointToLight = normalize(pLight.pos - pixelPos);
	float dist = length(pLight.pos - pixelPos);

	vec3 diffuse = vec3(0.0f);
	float specular = 0.0f;

	//~~ There's no need to do ANY calculations if we're out of range.
	if(dist < pLight.range)
	{
		//Calculate diffuse factor.
		float diffuseFactor = dot(pointToLight, aNormal);
		diffuseFactor = clamp(diffuseFactor, 0.0f, 1.0f);//Ensure value stays within defined range.

		//Calculate specular factor.
		vec3 refDir = normalize(reflect(-pointToLight, aNormal));
		float specularFactor = max(dot(viewDir, refDir),0.0);

		//Attenuation calculations.
		float attenuation = 1.0 / (pLight.constant + pLight.linear * dist + pLight.quadratic * (dist * dist));

		//Combine it all.
		//Add material diffuse and specular once we have a material set up.

		diffuse = max(pLight.diffuse * diffuseFactor, 0.0f); //*vec3(texture(material.diffuse, textureCoord));
		specular = pow(specularFactor, 64); //Replace 64 with material shininess once we have one.
											//vec3(texture(material.specular,textureCoord));

		diffuse *= attenuation;
		specular *= attenuation;
	}


	return (diffuse + specular) * pLight.power;
}

vec3 CalculateDirLight(DirectionalLight light, vec3 aNormal, vec3 viewDir)
{
	light.dir = normalize(light.dir);
	vec3 lightDirection = normalize(light.dir - fragPos);

	//Diffuse factor calculation.
	float diffuseFactor = max(dot(light.dir, aNormal), 0.0);

	//Specular factor calculation.
	//vec3 refDir = reflect(-lightDirection, aNormal);
	//float specularFactor = max(dot(viewDir, refDir), 0.0); //Replace 64 with material shininess once we have one.
	//Combine it all

	float diffuse = max(diffuseFactor, 0.0f);
	//float specular = pow(specularFactor, 64);

	return diffuse; //+ specular;
}

