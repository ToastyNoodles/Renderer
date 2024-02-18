#version 420 core
layout (location = 0) out vec4 lightTextureOut;

layout (binding = 0) uniform sampler2D albedoTexture;
layout (binding = 1) uniform sampler2D normalTexture;
layout (binding = 2) uniform sampler2D roughnessTexture;
layout (binding = 3) uniform sampler2D metallicTexture;
layout (binding = 4) uniform sampler2D positionTexture;

in vec2 f_uv;

struct PointLight
{
	vec3 position;
	vec3 color;
    float strength;
    float radius;
};

#define NUM_POINTLIGHT 6
uniform PointLight pointLights[NUM_POINTLIGHT];
uniform vec3 viewPos;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{	
    vec3 albedo = pow(vec3(texture(albedoTexture, f_uv)), vec3(2.2));
    vec3 normal = normalize(vec3(texture(normalTexture, f_uv)));
    float roughness = vec3(texture(roughnessTexture, f_uv)).r;
    float metallic = vec3(texture(metallicTexture, f_uv)).r;
    vec3 fragpos = vec3(texture(positionTexture, f_uv));

    vec3 V = normalize(viewPos - fragpos);
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < NUM_POINTLIGHT; i++) 
    {
        vec3 L = normalize(pointLights[i].position - fragpos);
        vec3 H = normalize(V + L);
        float distance = length(pointLights[i].position - fragpos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = pointLights[i].color * attenuation;

        float NDF = DistributionGGX(normal, H, roughness);   
        float G = GeometrySmith(normal, V, L, roughness);      
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator = NDF * G * F; 
        float denominator = 4.0 * max(dot(normal, V), 0.0) * max(dot(normal, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	  

        float NdotL = max(dot(normal, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }   
    
    vec3 ambient = vec3(0.1) * albedo;
    
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    lightTextureOut = vec4(color, 1.0);
}