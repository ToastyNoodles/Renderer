#version 330 core
out vec4 FragColor;

in vec2 f_uv;

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D specular;

void main()
{
	FragColor = texture(diffuse, f_uv);
}