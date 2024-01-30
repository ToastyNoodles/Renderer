#version 330 core
out vec4 FragColor;

in vec2 f_uv;

uniform sampler2D tex;

void main()
{
	FragColor = texture(tex, f_uv);
}