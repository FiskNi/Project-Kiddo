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

vec3 CalculatePointLight(PointLight light, vec3 pixelPos, vec3 aNormal, vec3 viewDir);
vec3 CalculateDirLight(DirectionalLight light, vec3 aNormal, vec3 viewDir);

void main () {
	vec4 diffuse = texture(diffuseTex, vec2(textureCoord.s, 1 - textureCoord.t));

	vec3 norm = normalize(normal);

	vec3 viewDirection = normalize(camPos - fragPos);

	vec3 ambientLight = diffuse.xyz * vec3(0.1f, 0.1f, 0.1f);
	vec3 directionalLight = vec3(0.0f);

	vec3 projLightCoords = shadow_coord.xyz / shadow_coord.w;
	projLightCoords = projLightCoords * 0.5 + 0.5;
	float shadowMapDepth = texture(shadowMap, projLightCoords.xy).r;
	float lightDepthValue = projLightCoords.z;
	float bias = 0.001f;
	lightDepthValue -= bias;

	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projLightCoords.xy + vec2(x, y) * texelSize).r; 
			directionalLight += diffuse.xyz * CalculateDirLight(dirLight, norm, viewDirection) * (lightDepthValue < pcfDepth ? 1.0f : 0.0f);
        }    
    }
	directionalLight /= 9;

	if (projLightCoords.z > 1.0f)
	{
		directionalLight = diffuse.xyz * CalculateDirLight(dirLight, norm, viewDirection);
	}

	vec3 pointLight = vec3(0.0f);
	for(int i = 0; i < 6; ++i)
	{
		pointLight += diffuse.xyz * CalculatePointLight(pointLights[i], fragPos, norm, viewDirection);
	}


	vec4 finalColor = clamp(vec4(ambientLight + directionalLight + pointLight, 1.0f), 0.0f,  1.0f);
	fragment_color = finalColor;
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
	vec3 specular = vec3(0.0f);

	//~~ There's no need to do ANY calculations if we're out of range.
	if (dist < pLight.range)
	{
		//Calculate diffuse factor.
		float diffuseFactor = max(dot(pointToLight, aNormal), 0.0f);

		//Calculate specular factor.
		vec3 refDir = normalize(reflect(-pointToLight, aNormal));
		float specularFactor = pow(max(dot(viewDir, refDir), 0.0), 64); //Replace 64 with material shininess once we have one.

		//Attenuation calculations.
		float attenuation = 1.0 / (pLight.constant + pLight.linear * dist + pLight.quadratic * (dist * dist));

		//Combine it all.
		//Add material diffuse and specular once we have a material set up.

		diffuse = pLight.diffuse * diffuseFactor; 
		specular = pLight.specular * specularFactor;

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
	// vec3 pointToLight = normalize(light.pos - fragPos);
	// vec3 refDir = reflect(-pointToLight, aNormal);
	// float specularFactor = pow(max(dot(viewDir, refDir), 0.0), 64); //Replace 64 with material shininess once we have one.
	//Combine it all

	float diffuse = diffuseFactor;

	return diffuse;
}

