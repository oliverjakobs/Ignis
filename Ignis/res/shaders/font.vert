#version 330 core
layout (location = 0) in vec4 aVertex; // <vec2 pos, vec2 tex>

uniform mat4 projection;

out vec2 texCoord;

void main()
{
    gl_Position = projection * vec4(aVertex.xy, 0.0, 1.0);
    texCoord = aVertex.zw;
}  