#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 mvp;

out vec2 texCoord;

void main()
{
	gl_Position = mvp * vec4(aPosition, 0.0f, 1.0f);
	texCoord = aTexCoord;
}