#version 410 core

const float GRAY_DARK = 0.40;   // darkest face
const float GRAY_SPAN = 0.28;   // lightest face = GRAY_DARK + GRAY_SPAN

in vec3 vObjectPosition;
flat in float vShade;

uniform sampler2D uTexture;
uniform vec2 uTextureScale;     // the shorter side of the image spans the model
uniform float uTextureMix;      // 0 = gray faces, 1 = texture, eased in between

out vec4 FragColor;

// The image projected along Z, as seen from the front.
vec3 planar(vec3 p)
{
	return texture(uTexture, 0.5 + 0.5 * p.xy * uTextureScale).rgb;
}

void main()
{
	vec3 gray = vec3(GRAY_DARK + GRAY_SPAN * vShade);

	FragColor = vec4(mix(gray, planar(vObjectPosition), uTextureMix), 1.0);
}
