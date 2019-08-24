#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec2 aOffset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;

void main()
{
	gl_Position = projection * view * model * vec4(aPosition + aOffset, 0.0f, 1.0f);
	texCoord = aTexCoord;
}