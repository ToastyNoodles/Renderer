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
	//Uses material color property as object color or texture tint depending on if albedo texture is used.
	if (texture(albedoTexture, fTexCoord).r == 0) {
		albedoTextureOut = vec4(albedo, 1.0);
	} else {
		albedoTextureOut = texture(albedoTexture, fTexCoord) * vec4(albedo, 1.0);
	}

	//If normal texture is used do tangent space normal calculations otherwise use object normals.
	if (texture(normalTexture, fTexCoord).r == 0) {
		normalTextureOut = vec4(fNormal, 1.0);
	} else {
		vec3 normalMap = texture(normalTexture, fTexCoord).rgb;
		mat3 tbn = mat3(normalize(fTangent), normalize(fBitangent), normalize(fNormal));
		vec3 normal = normalize(tbn * (normalMap.rgb * 2.0 - 1.0));
		normalTextureOut = vec4(normal, 1.0);
	}

	//Uses rma texture and material properties as strength modifiers if used otherwise uses specular and roughness values of material.
	if (texture(rmaTexture, fTexCoord).r == 0) {
		rmaTextureOut = vec4(roughness, specular, 1.0, 1.0);
	} else {
		vec3 rmaMap = texture(rmaTexture, fTexCoord).rgb;
		rmaTextureOut = vec4(rmaMap.r * roughness, rmaMap.g * specular, rmaMap.b, 1.0);
	}

	positionTextureOut = fWorldPos;
}