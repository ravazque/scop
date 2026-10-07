#version 410 core

layout (location = 0) in vec3 aPosition;

uniform mat4 uModel;        // object -> world
uniform mat4 uView;         // world -> eye
uniform mat4 uProjection;   // eye -> clip (perspective)

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}
