#version 330 core

out vec4 fColor;

uniform sampler2D uTexture;

in vec2 texCoord;

const float offset = 1.0 / 300.0;  

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    
    vec3 sample[9];
    for(int i = 0; i < 9; i++)
    {
        sample[i] = vec3(texture(uTexture, texCoord.st + offsets[i]));
    }

    vec3 color = vec3(0.0);
    for(int i = 0; i < 9; i++)
        color += sample[i] * kernel[i];
    
    fColor = vec4(color, 1.0);
}  