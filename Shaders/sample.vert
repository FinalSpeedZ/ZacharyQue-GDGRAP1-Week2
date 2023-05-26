#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 transform; // 4 by 4 transformation matrix

void main() {
	gl_Position = transform * vec4(aPos, 1.0);
}