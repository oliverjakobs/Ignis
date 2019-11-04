#version 330 core

out vec4 fColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

in vec3 lightDir;
in vec2 texCoord;
in mat3 TBN;

void main (void)
{
	//texture color 
    vec3 color = texture(diffuseMap, texCoord).rgb;

	// normal
	vec3 normal = TBN * normalize( texture2D( normalMap, texCoord.st ).xyz * 2.0 - 1.0 );
	float lambert = max(0.0, dot(normal, normalize(lightDir)));

	fColor = vec4(lambert * color, 1.0);
}