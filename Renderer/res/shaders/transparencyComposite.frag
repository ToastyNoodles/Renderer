#version 420 core
layout (location = 0) out vec4 FragColor;

in vec2 fTexCoord;

layout (binding = 0) uniform sampler2D frameTexture;
layout (binding = 1) uniform sampler2D transparencyTexture;

void main()
{
	vec3 frameColor = texture(frameTexture, fTexCoord).rgb; 
	vec3 glassColor = texture(transparencyTexture, fTexCoord).rgb;

	vec3 final = frameColor;

	if (glassColor != vec3(0.0))
	{
		final = glassColor;
	}

	FragColor = vec4(final, 1.0);
}