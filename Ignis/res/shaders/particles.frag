#version 430 core

layout (location = 0) out vec4 fColor;

in float fValue;

uniform vec4 particleColor;

void main(void)
{
	fColor = vec4(particleColor.xyz, fValue);
	//color = vec4(value, 1.0, 1.0, 1.0);
}