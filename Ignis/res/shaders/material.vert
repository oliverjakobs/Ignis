#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 mvp;

uniform vec3 lightPos;

out vec3 lightDir;
out vec2 texCoord;
out mat3 TBN;

void main()
{
	vec3 position = (model * vec4(aPosition, 1.0)).xyz;

	float w = (dot(cross(aTangent, aBitangent), aNormal) > 0.0) ? 1.0 : -1.0;
	
	vec3 t = normalize((model * vec4(aTangent.xyz, 0.0)).xyz );
	vec3 b = normalize((model * vec4((cross(aNormal, aTangent.xyz) * w), 0.0)).xyz);
	vec3 n = normalize((model * vec4(aNormal, 0.0)).xyz);
	TBN = mat3(t, b, n);

	lightDir = lightPos - position;
	
	texCoord = aTexCoord;
	gl_Position = mvp * vec4(aPosition, 1.0);
}