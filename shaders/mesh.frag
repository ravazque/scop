#version 410 core

const float GRAY_DARK = 0.40;   // darkest face
const float GRAY_SPAN = 0.28;   // lightest face = GRAY_DARK + GRAY_SPAN

flat in float vShade;

out vec4 FragColor;

void main()
{
	FragColor = vec4(vec3(GRAY_DARK + GRAY_SPAN * vShade), 1.0);
}
