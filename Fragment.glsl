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

layout(location = 15) uniform vec3 lightPos;
layout(location = 16) uniform vec3 camPos;

uniform sampler2D diffuseTex;

void main () {
	vec4 texSample = texture(diffuseTex, vec2(textureCoord.s, 1- textureCoord.t));
	//vec3 hardNorm = normalize(vec3(1.0f,0.0f,0.0f));
	vec3 hardNorm = normalize(normal);
	vec3 dirOfLight = normalize(lightPos-fragPos);

	float ambient = 0.2;
	//Diffuse calculation
	float diffuse = max(dot(hardNorm,dirOfLight),0.0);

	//specular calculation
	float specStr = 0.5;

	vec3 viewDir = normalize(camPos-fragPos);
	vec3 refDir = reflect(-dirOfLight, hardNorm);

	float specVal = pow(max(dot(viewDir,refDir),0.0),64);
	vec3 specular = specStr * specVal * vec3(0,0,1);//Replace hardcoded vec3 with lightColour later.

	vec3 newCol = (ambient+diffuse+specular)*vec3(1.0,0.0,0.0);

	fragment_color = vec4 (newCol, 1.0);

	//fragment_color = vec4 (color, 1.0);
	//fragment_color = vec4 (color * colourFromImGui, 1.0);
	//fragment_color = myAttrOut * vec4 (color * colourFromImGui, 1.0);
}