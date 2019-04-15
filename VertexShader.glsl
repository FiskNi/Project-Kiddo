#version 440
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_tanget;
layout(location = 4) in vec3 vertex_bitangent;


out vec3 color;
out vec2 textureCoord;
out vec4 shadow_coord;

// uniform offset
layout(location=10) uniform float offset;

layout(location=11) uniform mat4 rotateZ;

layout(location = 12) uniform mat4 view;
layout(location = 13) uniform mat4 proj;
layout(location = 14) uniform mat4 model;

uniform mat4 SHADOW_MAT;


out vec3 fragPos;
out vec3 normal;

void main() {
	normal= vertex_normal;
	//color = vertex_color;
	// gl_Position = newVertex;//vec4(vec3(newVertex.x + offset, newVertex.yz), 1.0);
	// gl_Position = vec4(vertex_position, 1.0);

	fragPos =vec3(model * vec4(vertex_position, 1.0));
	textureCoord = vec2(vertex_uv.x, vertex_uv.y);
 	vec4 newVertex = rotateZ * vec4(vertex_position, 1.0f);
// 	gl_Position = vec4(vec3(newVertex.x + offset, newVertex.yz), 1.0);
	gl_Position = proj*view*model*vec4(vertex_position, 1.0f);

	shadow_coord = SHADOW_MAT * (model * vec4(vertex_position, 1.0)); 
}