#version 420 core
layout (location = 2) in vec2 uv;

out vec2 f_uv;

void main()
{
	f_uv = uv;
}