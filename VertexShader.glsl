#version 440
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_tangent;
layout(location = 4) in vec3 vertex_bitangent;

// uniform offset
layout(location=10) uniform float offset;

layout(location=11) uniform mat4 rotateZ;

layout(location = 12) uniform mat4 view;
layout(location = 13) uniform mat4 proj;
layout(location = 14) uniform mat4 model;

uniform mat4 SHADOW_MAT;

out VS_OUT{
	vec3 position;
	vec2 uv;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;

	vec4 shadow_coord;
} vs_out;

void main() {
	vs_out.normal = vertex_normal;
	vs_out.position = vec3(model * vec4(vertex_position, 1.0));
	vs_out.uv = vec2(vertex_uv.x, vertex_uv.y);
	vs_out.tangent = vertex_tangent;
	vs_out.bitangent = vertex_bitangent;

	gl_Position = proj * view * model * vec4(vertex_position, 1.0f);
	vs_out.shadow_coord = SHADOW_MAT * (model * vec4(vertex_position, 1.0)); 
}