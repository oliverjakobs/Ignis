#version 330 core

layout(location = 0) out vec4 f_Color;

in vec3 v_Position;

uniform vec4 u_Color;

void main()
{
	f_Color = u_Color;
}