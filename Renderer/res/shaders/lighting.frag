#version 330 core
out vec4 FragColor;

in vec3 f_pos;
in vec3 f_norm;
in vec2 f_uv;
in vec3 f_tan;
in vec3 f_bitan;

uniform sampler2D albedo;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;

void main()
{
	float ambientStrength = 0.5;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(f_norm);
	vec3 lightDir = normalize(lightPosition - f_pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 1.0;
	vec3 viewDir = normalize(viewPosition - f_pos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec4 result = vec4(ambient + diffuse + specular, 1.0) * texture(albedo, f_uv);
	FragColor = result;
}