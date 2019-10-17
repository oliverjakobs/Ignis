#version 430 core

layout (location = 0) in vec4 aVert;

uniform mat4 mvp;

out float fValue;

void main(void)
{
	fValue = aVert.w;
	gl_Position = mvp * vec4(aVert.xy, 0.0f, 1.0);
}