#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 fragPos;
out vec2 texCoord;
out vec3 tangentLightPos;
out vec3 tangentViewPos;
out vec3 tangentFragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	fragPos = vec3(model * vec4(aPosition, 1.0));   
	texCoord = aTexCoord;

	float w = (dot(cross(aTangent, aBitangent), aNormal) > 0.0F) ? 1.0F : -1.0F;

	vec3 n = normalize((model * vec4(aNormal, 0.0)).xyz);
	vec3 t = normalize((model * vec4(aTangent.xyz, 0.0)).xyz);
	vec3 b = normalize((model * vec4((cross(aNormal, aTangent.xyz) * w), 0.0)).xyz);
	mat3 TBN = mat3( t, b, n );

	vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
	//mat3 TBN = mat3(T, B, N);

	tangentLightPos = TBN * lightPos;
	tangentViewPos  = TBN * viewPos;
	tangentFragPos  = TBN * fragPos;

	gl_Position = projection * view * model * vec4(aPosition, 1.0);
}