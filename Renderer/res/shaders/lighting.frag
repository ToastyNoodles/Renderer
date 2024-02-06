#version 420 core
layout (location = 0) out vec4 lightTextureOut;

layout (binding = 0) uniform sampler2D colorTexture;
layout (binding = 1) uniform sampler2D normalTexture;
layout (binding = 2) uniform sampler2D rmaTexture;
layout (binding = 3) uniform sampler2D positionTexture;

in vec2 f_uv;

struct PointLight
{
	vec3 position;
	vec3 color;
	float radius;
};

#define NUM_POINTLIGHT 3
uniform PointLight pointLights[NUM_POINTLIGHT];

uniform vec3 viewPosition;

void main()
{
	vec3 color = vec3(texture(colorTexture, f_uv));
	vec3 normal = vec3(texture(normalTexture, f_uv));
	vec3 rma = vec3(texture(rmaTexture, f_uv));
	vec3 fragPos = vec3(texture(positionTexture, f_uv));

	float roughness = rma.r;
	float metallic = rma.g;

	vec3 result = color * 0.1; //ambient light
	vec3 viewDir = normalize(viewPosition - fragPos);
	
	for (int i = 0; i < NUM_POINTLIGHT; i++)
	{
		float dist = length(pointLights[i].position - fragPos);
		if (dist < pointLights[i].radius)
		{
			//diffuse
			vec3 lightDir = normalize(pointLights[i].position - fragPos);
			vec3 diffuse = max(dot(normal, lightDir), 0.0) * color * pointLights[i].color;

			//specular
			vec3 halfwayDir = normalize(lightDir + viewDir);
			float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
			vec3 specular = pointLights[i].color * spec * rma;

			//attenuation
			float attenuation = 1.0 / (dist * dist);
			diffuse *= attenuation;
			specular *= attenuation;

			result += diffuse + specular;
		}
	}

	lightTextureOut = vec4(result, 1.0);
}