#version 420 core
layout (location = 0) in vec3 position;

out vec3 f_pos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	f_pos = position;
	vec4 pos = projection * view * vec4(position, 1.0);
	gl_Position = pos.xyww;
}