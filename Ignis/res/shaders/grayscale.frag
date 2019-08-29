#version 330 core

out vec4 fColor;

uniform sampler2D uTexture;

in vec2 texCoord;

void main()
{
    fColor = texture(uTexture, texCoord);
    float average = 0.2126 * fColor.r + 0.7152 * fColor.g + 0.0722 * fColor.b;
    fColor = vec4(average, average, average, 1.0);
}  