#version 420 core
out vec4 FragColor;

in vec4 fWorldPos;
in vec3 fNormal;
in vec2 fTexCoord;

layout (binding = 0) uniform sampler2D lightingTexture;
layout (binding = 1) uniform sampler2D glassTexture;

void main()
{
	vec3 lightingColor = texture(lightingTexture, fTexCoord).rgb;
	vec3 glassColor = texture(glassTexture, fTexCoord).rgb;

	vec3 result = lightingColor;
	
	if (glassColor != vec3(0, 0, 0))
	{
		result = glassColor;
	}

	FragColor = vec4(result, 1.0);
}