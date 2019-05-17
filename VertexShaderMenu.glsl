#version 440
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;
//layout(location = 2) in vec3 vertex_normal;
//layout(location = 3) in vec3 vertex_tangent;
//layout(location = 4) in vec3 vertex_bitangent;

//layout(location = 5) uniform mat4 view;
//layout(location = 6) uniform mat4 proj;
//layout(location = 7) uniform mat4 model;

//layout(location = 8) uniform mat4 SHADOW_MAT;

out VS_OUT
{
	vec3 position;
	vec2 uv;
//	vec3 normal;
//	vec3 tangent;
//	vec3 bitangent;
//
//	vec4 shadow_coord;
} vs_out;

void main() 
{	
	vs_out.position = vertex_position, 1.0;
	//vs_out.position = vec3(proj * view * model * vec4(vertex_position, 1.0f));

	vs_out.uv = vec2(vertex_uv.x, vertex_uv.y);

	//	vs_out.normal = transpose(inverse(mat3(model))) * vertex_normal;
	//	vs_out.tangent = transpose(inverse(mat3(model))) * vertex_tangent;
	//	vs_out.bitangent = transpose(inverse(mat3(model))) * vertex_bitangent;

	// The way matrices are applied here is important to print it to the screen in 2D space
	gl_Position = vec4(vertex_position, 1.0f); // proj * view * model * 

	//vs_out.shadow_coord = SHADOW_MAT * model * vec4(vertex_position, 1.0); 
}

//