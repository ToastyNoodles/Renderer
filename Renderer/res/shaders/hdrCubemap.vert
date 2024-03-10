#version 420 core
layout (location = 0) in vec3 aPosition;

out vec3 fPosition;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	fPosition = aPosition;
	gl_Position = projection * view * vec4(fPosition, 1.0);
}