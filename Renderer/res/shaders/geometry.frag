#version 420 core
layout (location = 0) out vec4 albedoTextureOut;
layout (location = 1) out vec4 normalTextureOut;
layout (location = 2) out vec4 roughnessTextureOut;
layout (location = 3) out vec4 metallicTextureOut;
layout (location = 4) out vec4 positionTextureOut;

layout (binding = 0) uniform sampler2D albedoTexture;
layout (binding = 1) uniform sampler2D normalTexture;
layout (binding = 2) uniform sampler2D roughnessTexture;
layout (binding = 3) uniform sampler2D metallicTexture;

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

	vec4 roughness = texture(roughnessTexture, fTexCoord);
	vec4 metallic = texture(metallicTexture, fTexCoord);
	
	albedoTextureOut = albedo;
	normalTextureOut =  vec4(normal, 1.0);
	roughnessTextureOut = roughness;
	metallicTextureOut = metallic;
	positionTextureOut = fWorldPos;
}