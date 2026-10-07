#version 410 core

out vec4 FragColor;

void main()
{
	// Golden-ratio steps spread consecutive triangles over subtle, distinct grays.
	float shade = 0.30 + 0.45 * fract(float(gl_PrimitiveID) * 0.618034);

	FragColor = vec4(vec3(shade), 1.0);
}
