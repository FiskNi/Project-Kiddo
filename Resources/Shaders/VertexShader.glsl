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

layout(location = 18) uniform bool hasAnimation;

layout(location = 7) uniform mat4 view;
layout(location = 8) uniform mat4 proj;
layout(location = 9) uniform mat4 model;

layout(location = 10) uniform mat4 SHADOW_MAT;


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

	if (hasAnimation)
	{
		gl_Position  = (boneMat[bones[0]] * vec4(vertex_position, 1.0f)) * weights.x;
		gl_Position += (boneMat[bones[1]] * vec4(vertex_position, 1.0f)) * weights.y;
		gl_Position += (boneMat[bones[2]] * vec4(vertex_position, 1.0f)) * weights.z;
		gl_Position += (boneMat[bones[3]] * vec4(vertex_position, 1.0f)) * weights.w;
	}
	
	vs_out.position		= vec3(model * gl_Position);
	vs_out.uv			= vec2(vertex_uv.x, vertex_uv.y);

	vs_out.normal = vertex_normal;
	if (hasAnimation)
	{
		vs_out.normal  = vec3((boneMat[bones[0]] * vec4(vertex_normal, 1.0f)) * weights.x);
		vs_out.normal += vec3((boneMat[bones[1]] * vec4(vertex_normal, 1.0f)) * weights.y);
		vs_out.normal += vec3((boneMat[bones[2]] * vec4(vertex_normal, 1.0f)) * weights.z);
		vs_out.normal += vec3((boneMat[bones[3]] * vec4(vertex_normal, 1.0f)) * weights.w);
	}
	vs_out.normal		=  normalize(transpose(inverse(mat3(model))) * vs_out.normal);

	vs_out.tangent = vertex_tangent;
	if (hasAnimation)
	{
		vs_out.tangent = vec3((boneMat[bones[0]] * vec4(vertex_tangent, 1.0f)) * weights.x);
		vs_out.tangent += vec3((boneMat[bones[1]] * vec4(vertex_tangent, 1.0f)) * weights.y);
		vs_out.tangent += vec3((boneMat[bones[2]] * vec4(vertex_tangent, 1.0f)) * weights.z);
		vs_out.tangent += vec3((boneMat[bones[3]] * vec4(vertex_tangent, 1.0f)) * weights.w);
	}
	vs_out.tangent		= normalize(transpose(inverse(mat3(model))) * vs_out.tangent);

	vs_out.bitangent	= vertex_bitangent;
	if (hasAnimation)
	{
		vs_out.bitangent = vec3((boneMat[bones[0]] * vec4(vertex_bitangent, 1.0f)) * weights.x);
		vs_out.bitangent += vec3((boneMat[bones[1]] * vec4(vertex_bitangent, 1.0f)) * weights.y);
		vs_out.bitangent += vec3((boneMat[bones[2]] * vec4(vertex_bitangent, 1.0f)) * weights.z);
		vs_out.bitangent += vec3((boneMat[bones[3]] * vec4(vertex_bitangent, 1.0f)) * weights.w);
	}
	vs_out.bitangent		=  normalize(transpose(inverse(mat3(model))) * vs_out.bitangent);


	vs_out.shadow_coord = SHADOW_MAT * model * gl_Position; 
	gl_Position			= proj * view * model * gl_Position;
}