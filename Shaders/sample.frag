#version 330 core

out vec4 FragColor;

// Texture to be passed
uniform sampler2D tex0;

// Should recieve the textCoord from the vertex shader
in vec2 texCoord;

void main() {
	// FragColor = vec4(1.0f, 0.5f, 0.25f, 1.0f);

	// Assign the texture color using the function
	FragColor = texture(tex0, texCoord);
}