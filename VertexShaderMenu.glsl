#version 440
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;


out VS_OUT
{
	vec3 position;
	vec2 uv;

} vs_out;

void main() 
{	
	vs_out.position = vertex_position, 1.0;

	vs_out.uv = vec2(vertex_uv.x, vertex_uv.y);

	// The way matrices are applied here is important to print it to the screen in 2D space
	gl_Position = vec4(vertex_position, 1.0f);

}

//