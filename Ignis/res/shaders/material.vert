#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 mvp;

uniform mat4 model;

out vec3 position;
out vec3 normal;
out vec2 texCoord;

void main()
{
	position = vec3(model * vec4(aPosition, 0.0));
    normal = mat3(transpose(inverse(model))) * aNormal;  
	texCoord = aTexCoord;

	gl_Position = mvp * vec4(aPosition, 1.0);
}
