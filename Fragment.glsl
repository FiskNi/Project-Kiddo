#version 440
// these values are interpolated at the rasteriser
in vec3 color;
in vec3 fragPos;
layout(location=3) in float myAttrOut;
in vec2 textureCoord;
in vec3 normal;

// this is the final pixel colour
out vec4 fragment_color;

// this is a uniform value, the very same value for ALL pixel shader executions
layout(location = 5) uniform vec3 colourFromImGui;

layout(location = 16) uniform vec3 camPos;

//Depth map for shadow mapping to sample 
uniform sampler2D shadowMap;
in vec4 shadow_coord;

float shadowCalc(vec4 shadow_coord, vec3 normal, vec3 light_pos);
vec4 calcDiffuse(vec3 light_pos, vec3 light_color, vec3 normal);

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

//float ambient = 0.2;

vec3 CalculatePointLight(PointLight light, vec3 pixelPos, vec3 aNormal, vec3 viewDir);
vec3 CalculateDirLight(DirectionalLight light, vec3 aNormal, vec3 viewDir);

void main () {
	vec4 texSample = texture(diffuseTex, vec2(textureCoord.s, 1- textureCoord.t));
	
	vec3 norm = normalize(normal);
	
	vec3 viewDirection = normalize(camPos -fragPos);

	vec3 totLightCalc = CalculateDirLight(dirLight,norm, viewDirection);

	for(int i=0;i<NR_P_LIGHTS;i++)
	{
		totLightCalc += CalculatePointLight(pointLights[i],fragPos,norm,viewDirection);
	}

	//vec3 newCol = totLightCalc*texSample;

	float shadow = shadowCalc(shadow_coord, normal, vec3(4.0, 6.0, 2.0));
	vec3 newCol = (totLightCalc+( - shadow))*texSample;

	fragment_color = vec4 (newCol, 1.0);

	//fragment_color = vec4 (color, 1.0);
	//fragment_color = vec4 (color * colourFromImGui, 1.0);
	//fragment_color = myAttrOut * vec4 (color * colourFromImGui, 1.0);
}

/*
=============================================================
Sample from the depth map for shadows. 
=============================================================
*/
float shadowCalc(vec4 shadow_coord, vec3 normal, vec3 light_pos){
	vec3 proj_coord = shadow_coord.xyz/shadow_coord.w;
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


=============================================================
*/
vec4 calcDiffuse(vec3 light_pos, vec3 light_color, vec3 normal){
	//Diffuse shading
	vec3 pointToLight = normalize(light_pos - fragPos.xyz);
	float diffuseFactor = dot(pointToLight, normal) / (length(pointToLight) * length(normal));
	diffuseFactor = clamp(diffuseFactor, 0, 1); //Make sure the diffuse factor isn't negative or above 1
	vec3 diffuse = diffuseFactor * light_color;
	vec4 final = vec4(diffuse, 1.0 );
	return final;
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
	//~~ There's no need to do ANY calculations if we're out of range.
	float dist = length(pLight.pos - pixelPos);

	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);
	float ambient = 0;

	if(dist<pLight.range)
	{
		//Calculate diffuse factor.
		float diffuseFactor = dot(pointToLight,aNormal);
		diffuseFactor = clamp(diffuseFactor , 0 , 1);//Ensure value stays within defined range.
		//Calculate specular factor.
		vec3 refDir = reflect(-pointToLight,aNormal);
		float specularFactor = pow(max(dot(viewDir,refDir),0.0), 64); //Replace 64 with material shininess once we have one.

		//Attenuation calculations.
		float attenuation = 1.0 / (pLight.constant + pLight.linear *dist +pLight.quadratic *(dist*dist));

		//Combine it all.
		//Add material diffuse and specular once we have a material set up.

		//vec3 ambient = pLight.ambient; //* vec3(texture(material.diffuse, textureCoord));
		diffuse = pLight.diffuse *diffuseFactor; //*vec3(texture(material.diffuse, textureCoord));
		specular = pLight.specular * specularFactor; //vec3(texture(material.specular,textureCoord));

		ambient = 0.2;

		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;
	}

	return (ambient+diffuse+specular)*pLight.power;
}

vec3 CalculateDirLight(DirectionalLight light, vec3 aNormal, vec3 viewDir)
{
	vec3 lightDirection = normalize(-light.dir);

	//Diffuse factor calculation.
	float diffuseFactor = max(dot(viewDir,aNormal),0.0);
	//Specular factor calculation.
	vec3 refDir = reflect(-lightDirection,aNormal);
	float specularFactor = pow(max(dot(viewDir,refDir),0.0), 64); //Replace 64 with material shininess once we have one.
	//Combine it all

	float ambient = 0.2;
	vec3 diffuse = light.col * diffuseFactor;
	vec3 specular = light.specular *specularFactor;

	return (ambient + diffuse + specular);
}

