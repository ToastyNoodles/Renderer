#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 uv;

out vec2 f_uv;

uniform mat4 viewProjection;
uniform mat4 model;

void main()
{
	f_uv = uv;
	gl_Position = viewProjection * model * vec4(position, 1.0);
}