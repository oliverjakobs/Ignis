#version 330 core

layout (location = 0) in vec2 a_Position;

uniform mat4 u_ViewProjection;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position, 0.0, 1.0);
}