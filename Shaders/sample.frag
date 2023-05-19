#version 330 core

out vec4 FragColor;

uniform float color;

void main() {
	float r_color, g_color, b_color;
	r_color = 1.f;
	g_color = 0.f;
	b_color = 0.5f;

	if (r_color - color <= 0.f) 
		r_color = 1.f;
	else 
		r_color = r_color - color;

	if (g_color + color >= 1.f) 
		g_color = 0.f;
	else 
		g_color = g_color + color;

	if (r_color >= 0.5f) {
		if (b_color + color >= 0.5f) 
			b_color = 0.5f;
		else 
			b_color = b_color + color;
	}

	else {
		if (b_color - color >= 0.5f) 
			b_color = 0.5f;
		else 
			b_color = b_color - color;	
	}
	FragColor = vec4(r_color, g_color, b_color, 1.0f);
}