#version 330 core

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec2 vOffset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition + vOffset, 0.0f, 1.0f);
	texCoord = vTexCoord;
}