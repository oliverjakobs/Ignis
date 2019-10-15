#version 430 core

layout (location = 0) out vec4 color;

in float value;

uniform vec4 particleColor;

void main(void)
{
	color = vec4(particleColor.xyz, value);
	//color = vec4(value, 1.0, 1.0, 1.0);
}