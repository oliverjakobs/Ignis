#version 330 core

out vec4 fColor;

uniform sampler2D uTexture;

in vec2 texCoord;

void main()
{
	vec4 texColor = texture(uTexture, texCoord);
	fColor = texColor;
}
