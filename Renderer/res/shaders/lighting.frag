#version 420 core
layout (location = 0) out vec4 lightTextureOut;

struct GlobalLight
{
	vec3 direction;
	vec3 color;
};

struct PointLight
{
	vec3 position;
	vec3 color;
    float strength;
    float radius;
};

layout (binding = 0) uniform sampler2D albedoTexture;
layout (binding = 1) uniform sampler2D normalTexture;
layout (binding = 2) uniform sampler2D rmaTexture;
layout (binding = 3) uniform sampler2D positionTexture;
layout (binding = 4) uniform samplerCube irradianceCubemap;

in vec2 fTexCoord;

#define MAX_POINTLIGHTS 32
uniform PointLight pointLights[MAX_POINTLIGHTS];
uniform GlobalLight globalLight;
uniform vec3 viewPos;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
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

vec3 CalculateGlobalLight(GlobalLight light, vec3 viewPos, vec3 worldPos, vec3 albedo, vec3 normal, float roughness, float metallic)
{
    vec3 N = normal;
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    vec3 V = normalize(viewPos - worldPos);
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(V + L);

    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    float NdotL = max(dot(N, L), 0.0);
    return ((kD * albedo / PI + specular) * NdotL) * globalLight.color;
}

vec3 CalculatePointLight(PointLight light, vec3 viewPos, vec3 worldPos, vec3 albedo, vec3 normal, float roughness, float metallic)
{
    vec3 N = normal;
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    vec3 V = normalize(viewPos - worldPos);
    vec3 L = normalize(light.position - worldPos);
    vec3 H = normalize(V + L);
    float attenuation = smoothstep(light.radius, 0.0, length(light.position - worldPos));
    vec3 radiance = light.color * light.strength;

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * radiance * attenuation * NdotL;
}

void main()
{	
    vec3 worldPos = vec3(texture(positionTexture, fTexCoord));
    vec3 albedo = pow(vec3(texture(albedoTexture, fTexCoord)), vec3(2.2));
    vec3 normal = normalize(vec3(texture(normalTexture, fTexCoord)));
    vec3 rma = vec3(texture(rmaTexture, fTexCoord));
    float roughness = clamp(rma.r, 0.025, 1.0);
    float metallic = clamp(rma.g, 0.025, 1.0);
    float ao = clamp(rma.b, 0.025, 1.0);
    
    //Lighting calculations
    vec3 Lo = CalculateGlobalLight(globalLight, viewPos, worldPos, albedo, normal, roughness, metallic);
    for (int i = 0; i < MAX_POINTLIGHTS; i++)
    {
        Lo += CalculatePointLight(pointLights[i], viewPos, worldPos, albedo, normal, roughness, metallic);
    }

    //Ambient lighting
    vec3 V = normalize(viewPos - worldPos);
    vec3 akS = fresnelSchlick(max(dot(normal, V), 0.0), vec3(0.04));
    vec3 akD = 1.0 - akS;
    akD *= 1.0 - metallic;
    vec3 irradiance = texture(irradianceCubemap, normal).rgb;
    vec3 diffuse = irradiance * albedo;
    vec3 ambient = (akD * diffuse) * ao;

    //Until I figure out how ibl works for now its just a 10% of the color.
    ambient = albedo * vec3(0.1);
    
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    lightTextureOut = vec4(color, 1.0);
}