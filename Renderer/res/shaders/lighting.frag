#version 420 core
layout (location = 0) out vec4 lightTextureOut;

layout (binding = 0) uniform sampler2D colorTexture;
layout (binding = 1) uniform sampler2D normalTexture;
layout (binding = 2) uniform sampler2D rmaTexture;
layout (binding = 3) uniform sampler2D positionTexture;

in vec2 f_uv;

struct DirectLight
{
	vec3 direction;
	vec3 color;
};

struct PointLight
{
	vec3 position;
	vec3 color;
    float linear;
    float quadratic;
};

#define NUM_POINTLIGHT 4
uniform PointLight pointLights[NUM_POINTLIGHT];
uniform DirectLight light;
uniform vec3 viewPos;
uniform float meta;

void main()
{
    vec3 color = vec3(texture(colorTexture, f_uv));
    vec3 normal = normalize(vec3(texture(normalTexture, f_uv)));
    vec3 rma = vec3(texture(rmaTexture, f_uv));
    vec3 fragpos = vec3(texture(positionTexture, f_uv));
    
    vec3 result = color * 0.1;
    vec3 viewDir  = normalize(viewPos - fragpos);

    //Direct light
    // diffuse
    vec3 lightDir = normalize(-light.direction);
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * color * light.color;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = rma * spec * light.color;

    result += diffuse + specular;

    //Point light
    for (int i = 0; i < NUM_POINTLIGHT; i++)
    {
        // diffuse
        vec3 lightDir = normalize(pointLights[i].position - fragpos);
        vec3 diffuse = max(dot(normal, lightDir), 0.0) * color * pointLights[i].color;

        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float specReflect = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
        vec3 specular = rma * specReflect * pointLights[i].color;
        
        //attenuation
        float dist = length(pointLights[i].position - fragpos);
        float attenuation = 1.0 / (1.0 + pointLights[i].linear * dist + pointLights[i].quadratic * dist * dist);

        diffuse *= attenuation;
        specular *= attenuation;
        result += diffuse + specular;
    }
    
    lightTextureOut = vec4(result, 1.0);
}