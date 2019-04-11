#version 440
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec2 vertex_normal;
layout(location = 3) in vec2 vertex_tanget;
layout(location = 4) in vec2 vertex_bitangent;


out vec3 color;
layout(location=3) out float myAttrOut;

// uniform offset
layout(location=10) uniform float offset;

layout(location=11) uniform mat4 rotateZ;

layout(location = 12) uniform mat4 view;
layout(location = 13) uniform mat4 proj;

void main() {
	// gl_Position = newVertex;//vec4(vec3(newVertex.x + offset, newVertex.yz), 1.0);
	// gl_Position = vec4(vertex_position, 1.0);

 	vec4 newVertex = rotateZ * vec4(vertex_position, 1.0f);
// 	gl_Position = vec4(vec3(newVertex.x + offset, newVertex.yz), 1.0);
	gl_Position = proj*view*vec4(vertex_position, 1.0f);
}