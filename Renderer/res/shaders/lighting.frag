#version 330 core
out vec4 FragColor;

in vec3 f_pos;
in vec3 f_norm;
in vec2 f_uv;
in vec3 f_tan;
in vec3 f_bitan;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;

vec4 PointLight()
{
	//attenuation
	vec3 lightVector = lightPosition - f_pos;
	float distance = length(lightVector);
	float a = 0.4;
	float b = 0.04;
	float attenuation = 1.0 / (a * distance * distance + b * distance + 1.0);
	
	//ambient
	float ambientStrength = 0.2;

	//diffuse lighting
	vec3 normal = normalize(f_norm);
	vec3 lightDirection = normalize(lightVector);
	float diffuse = max(dot(normal, lightDirection), 0.0);

	//specular lighting
	float specularStrength = 0.8;
	vec3 viewDirection = normalize(viewPosition - f_pos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);
	float specular = specAmount * specularStrength;

	return (texture(tex0, f_uv) * (diffuse * attenuation + ambientStrength) + texture(tex1, f_uv) * specular * attenuation) * vec4(lightColor, 0.0);
}

void main()
{
	FragColor = PointLight();
}