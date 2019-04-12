#version 440
// these values are interpolated at the rasteriser
in vec3 color;
in vec3 fragPos;
layout(location=3) in float myAttrOut;
in vec2 textureCoord;

// this is the final pixel colour
out vec4 fragment_color;

// this is a uniform value, the very same value for ALL pixel shader executions
layout(location = 5) uniform vec3 colourFromImGui;

layout(location = 15) uniform vec3 lightPos;
layout(location = 16) uniform vec3 camPos;

//Depth map for shadow mapping to sample 
uniform sampler2D shadowMap;
in vec4 shadow_coord;

float shadowCalc(vec4 shadow_coord, vec3 normal, vec3 light_pos);
vec4 calcDiffuse(vec3 light_pos, vec3 light_color, vec3 normal);

uniform sampler2D diffuseTex;


void main () {
	vec4 texSample = texture(diffuseTex, vec2(textureCoord.s, 1- textureCoord.t));
	vec3 hardNorm = normalize(vec3(1.0f,0.0f,0.0f));

	vec3 dirOfLight = normalize(lightPos-fragPos);

	float ambient = 0.2;
	//Diffuse calculation
	float diffuse = max(dot(hardNorm,dirOfLight),0.0);

	//specular calculation
	float specStr = 0.5;

	vec3 viewDir = normalize(camPos-fragPos);
	vec3 refDir = reflect(-dirOfLight, hardNorm);

	float specVal = pow(max(dot(viewDir,refDir),0.0),32);
	vec3 specular = specStr * specVal * vec3(0,0,1);//Replace hardcoded vec3 with lightColour later.

	vec3 newCol = (ambient+diffuse+specular)*vec3(1.0,0.0,0.0);

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

