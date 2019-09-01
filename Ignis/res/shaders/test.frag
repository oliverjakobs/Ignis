#version 330 core

out vec4 fColor;

uniform sampler2D uTexture;

in vec2 texCoord;

void main()
{
	fColor = texture(uTexture, texCoord);
}
