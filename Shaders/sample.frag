/* 
Point Light
1. Get distance of lightPos to fragment
2. Get intensity using formula
3. Multiply a multiplier to intensity (for brightness)
4. Applying the intensity to fragColor
*/
#version 330 core
// Color to be returned
out vec4 FragColor;

// Texture to be passed
uniform sampler2D tex0;

// Position of the light source
uniform vec3 lightPos;

// Color of the light: rgb
uniform vec3 lightColor;

// Strength of ambient light
uniform float ambientStr;

// Color of the ambient light
uniform vec3 ambientColor;

// Camera position
uniform vec3 cameraPos;

// Specular strength
uniform float specStr;

// Specular Phong
uniform float specPhong;

// Should recieve the texCoord from the vertex shader
in vec2 texCoord;

// Receive the processed normals from the vertex shader
in vec3 normCoord;

// Receive the position of the vertex to the vertex shader 
in vec3 fragPos;

void main() {

	// Transparent Pixels
	vec4 pixelColor = texture(tex0, texCoord);
	if (pixelColor.a < 0.1) 
		discard;

   // Normalize the received normals
   vec3 normal = normalize(normCoord);

   // Get the direction of the light to the fragment
   vec3 lightDir = normalize(lightPos -  fragPos);

   // Apply the diffuse formula here
   float diff = max(dot(normal, lightDir), 0.0);

   // Multiply it to the desired light color 
   vec3 diffuse = diff * lightColor;

   // Get the ambient light
   vec3 ambientCol = ambientColor * ambientStr;

   // Get our view direction from the camera to the fragment
   vec3 viewDir = normalize(cameraPos - fragPos);

   // Get the reflection vector
   vec3 reflectDir = reflect(-lightDir, normal);

   // Get the specular light 
   float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
   vec3 specColor = spec * specStr * lightColor;

   /* Point light computations */
   // Get distance of lightPos to fragment
   float distance = length(lightPos - fragPos);
   // Get intensity using formula
   float intensity = 1.f / (distance * distance);
   // Intensity Multiplier
   float multiplier = 100.f;
   intensity *= multiplier;
   // Assign the texture color using the function
   // Apply the lighting
   FragColor = vec4(intensity * (specColor + diffuse + ambientCol), 1.f) * texture(tex0, texCoord);
}