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

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
};

struct PointLight
{
	vec3 position;
	vec3 color;
};

uniform vec3 viewPosition;
uniform DirectionalLight sun;
uniform PointLight pointLights[NUM_POINTLIGHT];
uniform Material material;

vec3 CalcDirectionalLight(vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(-sun.direction);

	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = (sun.color * 0.1) * vec3(texture(material.diffuse, f_uv));
	vec3 diffuse = (diff * sun.color) * vec3(texture(material.diffuse, f_uv));
	vec3 specular = (spec * sun.color) * vec3(texture(material.specular, f_uv)).r;

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	//attenuation
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (dist * dist); //quadratic
	//float attenuation = 1.0 / dist; //linear

	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	//specular
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	vec3 diffuseColor = pow(vec3(texture(material.diffuse, f_uv)), vec3(2.2));
	vec3 ambient = ((light.color * 0.1) * attenuation) * diffuseColor;
	vec3 diffuse = ((diff * light.color) * attenuation) * diffuseColor;
	vec3 specular = ((spec * light.color) * attenuation) * vec3(texture(material.specular, f_uv)).r;

	return (ambient + diffuse + specular);
}

void main()
{
	vec3 norm = normalize(f_norm);
	vec3 viewDir = normalize(viewPosition - f_pos);
	vec3 result = CalcDirectionalLight(norm, f_pos, viewDir);

	for (int i = 0; i < NUM_POINTLIGHT; i++)
		result += CalcPointLight(pointLights[i], norm, f_pos, viewDir); 

	vec3 fragColor = result;
	float gamma = 2.2;
	FragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
}