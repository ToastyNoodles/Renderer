#version 330 core
out vec4 FragColor;

in vec3 f_pos;
in vec3 f_norm;
in vec2 f_uv;
in vec3 f_tan;
in vec3 f_bitan;

#define NUM_POINTLIGHT 3

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct PointLight
{
	vec3 position;
	vec3 color;
	float quadratic;
	float linear;
	float constant;
};

uniform vec3 viewPosition;
uniform PointLight pointLights[NUM_POINTLIGHT];
uniform Material material;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	//attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = ((light.color * 0.1) * attenuation) * vec3(texture(material.diffuse, f_uv));
	vec3 diffuse = ((diff * light.color) * attenuation) * vec3(texture(material.diffuse, f_uv));
	vec3 specular = ((spec * light.color) * attenuation) * vec3(texture(material.specular, f_uv)).r;

	return (ambient + diffuse + specular);
}

void main()
{
	vec3 norm = normalize(f_norm);
	vec3 viewDir = normalize(viewPosition - f_pos);
	vec3 result; //set this to directonal light when added

	for (int i = 0; i < NUM_POINTLIGHT; i++)
		result += CalcPointLight(pointLights[i], norm, f_pos, viewDir); 

	FragColor = vec4(result, 1.0);
}