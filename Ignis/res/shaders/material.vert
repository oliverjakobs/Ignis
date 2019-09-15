#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;

out vec3 fragPos;
out vec2 texCoord;
out vec3 tangentLightPos;
out vec3 tangentViewPos;
out vec3 tangentFragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	fragPos = vec3(M * vec4(aPosition, 1.0));   
	texCoord = aTexCoord;

	mat3 normalMatrix = transpose(inverse(mat3(M)));
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 N = normalize(normalMatrix * aNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N));    
	tangentLightPos = TBN * lightPos;
	tangentViewPos  = TBN * viewPos;
	tangentFragPos  = TBN * fragPos;

	gl_Position = MVP * vec4(aPosition, 1.0);
}
