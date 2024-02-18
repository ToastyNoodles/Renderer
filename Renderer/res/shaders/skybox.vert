#version 420 core
layout (location = 0) in vec3 aPosition;

out vec3 fTexCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	fTexCoord = aPosition;
	vec4 position = projection * view * vec4(aPosition, 1.0);
	gl_Position = position.xyww;
}