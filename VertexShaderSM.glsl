#version 440
layout(location = 0) in vec3 vertex_position;

layout (location = 1) uniform mat4 MODEL_MAT;
layout (location = 2) uniform mat4 SHADOW_MAT; //Projection and view


void main() {

	gl_Position = SHADOW_MAT * MODEL_MAT * vec4(vertex_position, 1.0);

}
