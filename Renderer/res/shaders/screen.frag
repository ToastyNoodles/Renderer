#version 420 core
out vec4 FragColor;

in vec2 f_uv;

layout (binding = 0) uniform sampler2D inputTexture;

void main()
{
	FragColor = texture(inputTexture, f_uv);
}