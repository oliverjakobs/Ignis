#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 mvp;

uniform mat4 model;

out vec2 texCoord;
out vec3 normal;

void main()
{
	gl_Position = mvp * vec4(aPosition, 1.0);
	texCoord = aTexCoord;
	normal = (model * vec4(aNormal, 0.0)).xyz;
}
