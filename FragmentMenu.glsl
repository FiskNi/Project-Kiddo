#version 440
// Vertexshader outputs
in VS_OUT
{
	vec3 position;
	vec2 uv;

} fsInput;

// this is the final pixel colour
out vec4 fragment_color;


// Texture inputs 
//uniform sampler2D shadowMap;
uniform sampler2D diffuseTex;

void main () 
{

	vec4 diffuse = texture(diffuseTex, vec2(fsInput.uv.s, 1 - fsInput.uv.t));

	//fragment_color = finalColor;
	fragment_color = vec4(diffuse, 1.0f);
	//fragment_color = vec4(fsInput.uv, 0.0f, 1.0f);

}

