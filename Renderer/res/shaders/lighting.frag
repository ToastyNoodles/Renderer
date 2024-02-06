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
    float linear;
    float quadratic;
	float radius;
};

#define NUM_POINTLIGHT 3
uniform PointLight pointLights[NUM_POINTLIGHT];

uniform vec3 viewPosition;

vec3 CalcPointLight(PointLight light, vec3 pos, vec3 norm, vec3 diff, vec3 spec)
{
    // attenuation
    float dist = length(light.position - pos);
    float attenuation = 1.0 / (1.0 + light.linear * dist + light.quadratic * dist * dist);

    //ambient
    vec3 ambient = diff * 0.1;
    vec3 viewDir  = normalize(viewPosition - pos);

    // diffuse
    vec3 lightDir = normalize(light.position - pos);
    vec3 diffuse = max(dot(norm, lightDir), 0.0) * diff * light.color;

    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 specular = light.color * spec;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 lighting = (ambient + diffuse + specular);
    return lighting;
}

void main()
{
    vec3 FragPos = texture(positionTexture, f_uv).rgb;
    vec3 Normal = texture(normalTexture, f_uv).rgb;
    vec3 Diffuse = texture(colorTexture, f_uv).rgb;
    vec3 Specular = texture(rmaTexture, f_uv).rgb;
    
    vec3 result;
    for(int i = 0; i < NUM_POINTLIGHT; i++)
        result += CalcPointLight(pointLights[i], FragPos, Normal, Diffuse, Specular);
    
    lightTextureOut = vec4(result, 1.0);
}