#version 330 core

out vec4 fColor;

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	sampler2D diffuse;
	sampler2D normal;
	sampler2D specular;
	float shininess;
};

in vec3 lightDir;
in vec3 viewDir;
in vec2 texCoord;
in mat3 TBN;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;


void main (void)
{
	//texture color 
    vec3 color = texture(material.diffuse, texCoord).rgb;

	// normal
	vec3 normal = TBN * normalize(texture2D(material.normal, texCoord).xyz * 2.0 - 1.0);
	float lambert = max(0.0, dot(normal, lightDir));

	// ambient
    vec3 ambient = light.ambient * color;
  	
    // diffuse 
	vec3 diffuse = light.diffuse * (lambert * color);

	// specular
	vec3 reflectDir = reflect(-lightDir, normal);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * vec3(0.5, 0.5, 0.5));  

	fColor = vec4(ambient + diffuse + specular, 1.0);
}