#version 410 core

const float GRAY_DARK = 0.40;           // darkest face
const float GRAY_SPAN = 0.28;           // lightest face = GRAY_DARK + GRAY_SPAN
const float TRIPLANAR_SHARPNESS = 4.0;  // higher: each face takes more of its main axis' projection

in vec3 vObjectPosition;
flat in vec3 vObjectNormal;
flat in float vShade;

uniform sampler2D uTexture;
uniform vec2 uTextureScale;     // the shorter side of the image spans the model
uniform float uTextureMix;      // 0 = gray faces, 1 = texture, eased in between
uniform float uTriplanar;       // 0 = planar along Z, 1 = triplanar, eased in between

out vec4 FragColor;

vec3 image(vec2 p)
{
	return texture(uTexture, 0.5 + 0.5 * p * uTextureScale).rgb;
}

// One projection along Z: faces that turn sideways stretch the image.
vec3 planar(vec3 p)
{
	return image(p.xy);
}

// A projection along each axis, blended by the face normal: no face is stretched.
// Each projection is oriented so the image reads upright and unmirrored from outside the model.
vec3 triplanar(vec3 p, vec3 n)
{
	vec3 w = pow(abs(n), vec3(TRIPLANAR_SHARPNESS));
	vec3 s = step(0.0, n) * 2.0 - 1.0;

	w /= w.x + w.y + w.z;
	return image(vec2(-p.z * s.x, p.y)) * w.x
		+ image(vec2(p.x, -p.z * s.y)) * w.y
		+ image(vec2(p.x * s.z, p.y)) * w.z;
}

void main()
{
	vec3 gray = vec3(GRAY_DARK + GRAY_SPAN * vShade);
	vec3 textured = mix(planar(vObjectPosition), triplanar(vObjectPosition, vObjectNormal), uTriplanar);

	FragColor = vec4(mix(gray, textured, uTextureMix), 1.0);
}
