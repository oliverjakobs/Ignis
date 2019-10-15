#version 430 core

in vec4 vert;

uniform mat4 mvp;

out float value;

void main(void)
{
	value = vert.w;
	gl_Position = mvp * vec4(vert.xy, 0.0f, 1.0);
}