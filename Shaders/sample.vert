#version 330 core

// location == 0 is from VertexAttributePointr first attribute
layout(location = 0) in vec3 aPos;

// The tex coord / UV is at 0
layout(location = 2) in vec2 aTex;

// Pass the tex coord to fragment shader
out vec2 texCoord;

uniform mat4 transform; // 4 by 4 transformation matrix

uniform mat4 projection;

uniform mat4 view;

void main() {
	gl_Position = projection * view * transform * vec4(aPos, 1.0);

	// Assign the UV
	texCoord = aTex;
}