#version 410 core

layout (location = 0) in vec3 aPosition;    // object space, inside the unit sphere
layout (location = 1) in vec3 aNormal;      // face normal, object space
layout (location = 2) in float aShade;      // gray level of the .obj face, in [0, 1)

uniform mat4 uModel;        // object -> world
uniform mat4 uView;         // world -> eye
uniform mat4 uProjection;   // eye -> clip (perspective)

out vec3 vObjectPosition;
flat out float vShade;

void main()
{
	vObjectPosition = aPosition;
	vShade = aShade;
	gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}
