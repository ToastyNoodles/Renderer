#version 420 core
out vec4 FragColor;

in vec2 fTexCoord;

layout (binding = 0) uniform sampler2D renderTexture;

void main()
{
	FragColor = texture(renderTexture, fTexCoord);
}