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
uniform float lightStrength;
uniform float lightRadius;
uniform float lightIntensity;
uniform vec3 viewPosition;

vec4 PointLight()
{
	//attenuation
	vec3 lightVector = lightPosition - f_pos;
	float distance = length(lightVector);
	float attenuation = 1.0 / (lightStrength * distance * distance + lightRadius * distance + lightIntensity);
	
	//ambient
	float ambientStrength = 0.2;

	//diffuse lighting
	vec3 normal = normalize(f_norm);
	vec3 lightDirection = normalize(lightVector);
	float diff = max(dot(normal, lightDirection), 0.0);

	//specular lighting
	vec3 viewDirection = normalize(viewPosition - f_pos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);

	return (texture(tex0, f_uv) * ((diff * vec4(lightColor, 1.0) * attenuation) + ambientStrength) + texture(tex1, f_uv).r * (spec * vec4(lightColor, 1.0) * attenuation));
}

void main()
{
	FragColor = PointLight();
}