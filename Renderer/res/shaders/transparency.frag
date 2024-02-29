#version 420 core

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

out vec4 FragColor;

in vec4 fWorldPos;
in vec3 fNormal;
in vec2 fTexCoord;

layout (binding = 0) uniform sampler2D albedoTexture;
layout (binding = 1) uniform sampler2D normalTexture;
layout (binding = 2) uniform sampler2D rmaTexture;
layout (binding = 3) uniform sampler2D positionTexture;
layout (binding = 4) uniform sampler2D lightingTexture;

#define MAX_POINTLIGHTS 32
uniform PointLight pointLights[MAX_POINTLIGHTS];
uniform GlobalLight globalLight;
uniform vec3 viewPos;

const float PI = 3.14159265359;

float D_GGX(float NoH, float roughness) 
{
  float alpha = roughness * roughness;
  float alpha2 = alpha * alpha;
  float NoH2 = NoH * NoH;
  float b = (NoH2 * (alpha2 - 1.0) + 1.0);
  return alpha2 / (PI * b * b);
}

float G1_GGX_Schlick(float NdotV, float roughness) 
{
  float r = (roughness + 1.0);
  float k = (r * r) / 8.0;
  float denom = NdotV * (1.0 - k) + k;
  return NdotV / denom;
}

float G_Smith(float NoV, float NoL, float roughness) 
{
  float g1_l = G1_GGX_Schlick(NoL, roughness);
  float g1_v = G1_GGX_Schlick(NoV, roughness);
  return g1_l * g1_v;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) 
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 MicrofacetBRDF(vec3 lightDir, vec3 viewDir, vec3 fNormal, vec3 fAlbedo, float fMetallic, float fRoughness) 
{
  vec3 halfDir = normalize(viewDir + lightDir);
  float NoV = clamp(dot(fNormal, viewDir), 0.0, 1.0);
  float NoL = clamp(dot(fNormal, lightDir), 0.0, 1.0);
  float NoH = clamp(dot(fNormal, halfDir), 0.0, 1.0);
  float VoH = clamp(dot(viewDir, halfDir), 0.0, 1.0);      
  
  // F0 for dielectics in range [0.0, 0.16] 
  // default FO is (0.16 * 0.5^2) = 0.04
  vec3 f0 = vec3(0.16 * (0.5 * 0.5));

  // in case of metals, baseColor contains F0
  f0 = mix(f0, fAlbedo, fMetallic);
  // specular microfacet (cook-torrance) BRDF
  vec3 F = fresnelSchlick(VoH, f0);
  float D = D_GGX(NoH, fRoughness);
  float G = G_Smith(NoV, NoL, fRoughness);
  vec3 spec = (D * G * F) / max(4.0 * NoV * NoL, 0.001);  
  // diffuse
  vec3 notSpec = vec3(1.0) - F; // if not specular, use as diffuse
  notSpec *= 1.0 - fMetallic; // no diffuse for metals
  vec3 diff = notSpec * fAlbedo / PI;   
  spec *= 1.05;
  vec3 result = diff + spec;
  return result;
}

vec3 CalculatePointLight(PointLight light, vec3 fWorldPos, vec3 fAlbedo, vec3 fNormal, float fRoughness, float fMetallic)
{
	vec3 viewDir = normalize(viewPos - fWorldPos);
    vec3 lightDir = normalize(light.position - fWorldPos);
	float lightRadiance = light.strength * 1;

    float attenuation = smoothstep(light.radius, 0, length(light.position - fWorldPos));
	float irradiance = max(dot(lightDir, fNormal), 0.0);

	irradiance *= attenuation * lightRadiance;
	vec3 brdf = MicrofacetBRDF(lightDir, viewDir, fNormal, fAlbedo, fMetallic, fRoughness);
    return brdf * irradiance * clamp(light.color, 0, 1);
}

vec3 CalculateGlobalLight(GlobalLight light, vec3 fWorldPos, vec3 fAlbedo, vec3 fNormal, float fRoughness, float fMetallic)
{
    vec3 viewDir = normalize(viewPos - fWorldPos);
	vec3 lightDir = normalize(-light.direction);
    vec3 brdf = MicrofacetBRDF(lightDir, viewDir, fNormal, fAlbedo, fMetallic, fRoughness);
    return brdf * light.color;
}

void main()
{
    vec3 fragpos = fWorldPos.xyz;
    vec3 albedo = pow(vec3(texture(albedoTexture, fTexCoord)), vec3(2.2));
    vec3 normal = fNormal;
    vec3 rma = vec3(texture(rmaTexture, fTexCoord));

    float roughness = rma.r;
    float metallic = rma.g;
    float ao = rma.b;
    
    vec3 lighting = CalculateGlobalLight(globalLight, fragpos, albedo, normal, roughness, metallic);
    for(int i = 0; i < MAX_POINTLIGHTS; i++) 
    {
        lighting += CalculatePointLight(pointLights[i], fragpos, albedo, normal, roughness, metallic);
    }   
    
    vec3 ambient = albedo * globalLight.color * vec3(0.1);
    vec3 color = (ambient + lighting) * ao;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
}