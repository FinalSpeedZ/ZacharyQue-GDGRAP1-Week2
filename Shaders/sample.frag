#version 330 core

out vec4 FragColor;

// Texture to be passed
uniform sampler2D tex0;

// Position of the light source
uniform vec3 lightPos;

// Color of the light: rgb
uniform vec3 lightColor;

// Should recieve the textCoord from the vertex shader
in vec2 texCoord;

// Receive the processed normals from the vertex shader
in vec3 normCoord;

// Receive the position of the vertex to the vertex shader 
in vec3 fragPos;

void main() {
	// Normalize the received normals
	vec3 normal = normalize(normCoord);

	// Get the direction of the light to the fragment
	vec3 lightDir = normalize(lightPos -  fragPos);

	// Apply the diffuse formula here
	float diff = max(dot(normal, lightDir), 0.0);

	// Multiply it to the desired light color
	vec3 diffuse = diff * lightColor;

	// Changing the colors using rgba values
	// FragColor = vec4(1.0f, 0.5f, 0.25f, 1.0f);

	// Assign the texture color using the function
	// Apply the diffuse
	FragColor = vec4(diffuse, 1.0) * texture(tex0, texCoord);
}