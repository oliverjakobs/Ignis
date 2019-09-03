#version 330 core

out vec4 fColor;

uniform sampler2D uTexture;

uniform vec3 lightDirection;

in vec2 texCoord;
in vec3 normal;

void main()
{
	fColor = texture2D(uTexture, texCoord);
}
