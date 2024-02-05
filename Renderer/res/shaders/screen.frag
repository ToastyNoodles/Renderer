#version 330 core
out vec4 FragColor;

in vec2 f_uv;

uniform sampler2D inputTexture;

void main()
{
	FragColor = texture(inputTexture, f_uv);
}