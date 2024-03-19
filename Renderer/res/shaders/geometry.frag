#version 420 core
layout (location = 0) out vec4 albedoTextureOut;
layout (location = 1) out vec4 normalTextureOut;
layout (location = 2) out vec4 rmaTextureOut;
layout (location = 3) out vec4 positionTextureOut;

uniform vec3 albedo;
uniform float specular;
uniform float roughness;
layout (binding = 0) uniform sampler2D albedoTexture;
layout (binding = 1) uniform sampler2D normalTexture;
layout (binding = 2) uniform sampler2D rmaTexture;

in vec4 fWorldPos;
in vec3 fNormal;
in vec2 fTexCoord;
in vec3 fTangent;
in vec3 fBitangent;

void main()
{
	vec4 albedoMap = texture(albedoTexture, fTexCoord) + vec4(albedo, 1.0);
	vec4 normalMap = texture(normalTexture, fTexCoord);
	mat3 tbn = mat3(normalize(fTangent), normalize(fBitangent), normalize(fNormal));
	//vec3 normal = normalize(tbn * (normalMap.rgb * 2.0 - 1.0));
	vec4 rmaMap = texture(rmaTexture, fTexCoord);
	vec4 rma = vec4(roughness, specular, 0.0, 1.0);
	
	albedoTextureOut = albedoMap;
	normalTextureOut =  vec4(normalize(fNormal), 1.0);
	rmaTextureOut = rma;
	positionTextureOut = fWorldPos;
}