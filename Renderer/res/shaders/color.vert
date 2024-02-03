#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 viewProjection;
uniform mat4 model;
uniform vec3 color;

out vec3 f_color;

void main()
{
	f_color = color;
	gl_Position = viewProjection * model * vec4(position, 1.0);
}