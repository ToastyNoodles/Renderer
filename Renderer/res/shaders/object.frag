#version 330 core
out vec4 FragColor;

in vec3 f_pos;
in vec3 f_norm;
in vec2 f_uv;
in vec3 f_tan;
in vec3 f_bitan;

void main()
{
	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}