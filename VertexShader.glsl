#version 440
layout(std140, binding  = 1) uniform SkinDataBlock
{
	mat4 boneMat[64];
};

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_tangent;
layout(location = 4) in vec3 vertex_bitangent;
layout(location = 5) in vec4 weights;
layout(location = 6) in ivec4 bones;

layout(location = 7) uniform mat4 view;
layout(location = 8) uniform mat4 proj;
layout(location = 9) uniform mat4 model;

layout(location = 10) uniform mat4 SHADOW_MAT;


//layout(location = 18) uniform skinningData skinData[64];

out VS_OUT
{
	vec3 position;
	vec2 uv;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;

	vec4 shadow_coord;
} vs_out;

void main() 
{	
	gl_Position = vec4(vertex_position, 1.0f);

	gl_Position  = (boneMat[bones[0]] * vec4(vertex_position, 1.0f)) * weights.x;
	gl_Position += (boneMat[bones[1]] * vec4(vertex_position, 1.0f)) * weights.y;
	gl_Position += (boneMat[bones[2]] * vec4(vertex_position, 1.0f)) * weights.z;
	gl_Position += (boneMat[bones[3]] * vec4(vertex_position, 1.0f)) * weights.w;
	
	vs_out.position = vec3(model * gl_Position);
	
	vs_out.uv = vec2(vertex_uv.x, vertex_uv.y);

	vs_out.normal = transpose(inverse(mat3(model))) * vertex_normal;
	vs_out.tangent = transpose(inverse(mat3(model))) * vertex_tangent;
	vs_out.bitangent = transpose(inverse(mat3(model))) * vertex_bitangent;

	gl_Position = proj * view * model * gl_Position;

	vs_out.shadow_coord = SHADOW_MAT * model * vec4(vertex_position, 1.0); 
}