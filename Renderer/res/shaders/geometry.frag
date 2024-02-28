#version 420 core
layout (location = 0) out vec4 albedoTextureOut;
layout (location = 1) out vec4 normalTextureOut;
layout (location = 2) out vec4 rmaTextureOut;
layout (location = 3) out vec4 positionTextureOut;

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
	vec4 albedo = texture(albedoTexture, fTexCoord);
	vec4 normalMap = texture(normalTexture, fTexCoord);
	mat3 tbn = mat3(normalize(fTangent), normalize(fBitangent), normalize(fNormal));
	vec3 normal = normalize(tbn * (normalMap.rgb * 2.0 - 1.0));
	vec4 rma = texture(rmaTexture, fTexCoord);
	
	albedoTextureOut = albedo;
	normalTextureOut =  vec4(normal, 1.0);
	rmaTextureOut = rma;
	positionTextureOut = fWorldPos;
}