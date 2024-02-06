#version 420 core
out vec4 FragColor;

in vec2 f_uv;

layout (binding = 0) uniform sampler2D renderTexture;

void main()
{
	FragColor = texture(renderTexture, f_uv);
}