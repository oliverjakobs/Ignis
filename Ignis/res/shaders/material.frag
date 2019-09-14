#version 330 core

out vec4 fColor;

uniform sampler2D uTexture;

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

in vec3 position;
in vec3 normal;
in vec2 texCoord;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;

	// diffuse 
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;  
	
	// specular
	vec3 viewDir = normalize(viewPos - position);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;  
	    
	vec3 result = ambient + diffuse + specular;
	fColor = vec4(result, 1.0);
}
