#version 420 core
layout (location = 0) out vec4 FragColor;

in vec2 fTexCoord;

layout (binding = 0) uniform sampler2D frameTexture;
layout (binding = 1) uniform sampler2D transparencyTexture;

void main()
{
	vec4 frameColor = texture(frameTexture, fTexCoord); 
	vec4 glassColor = texture(transparencyTexture, fTexCoord);

	FragColor = vec4(mix(glassColor.rgb, frameColor.rgb, glassColor.a), 1.0);
}