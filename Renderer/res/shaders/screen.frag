#version 330 core
out vec4 FragColor;

in vec2 f_uv;

uniform sampler2D screenTexture;

void main()
{
	FragColor = texture(screenTexture, f_uv);
}