#version 420 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvs;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 f_worldPos;
out vec3 f_normal;
out vec2 f_uvs;

void main()
{
	f_worldPos = model * vec4(position, 1.0);
	f_normal = normal;
	f_uvs = uvs;

	gl_Position = projection * view * f_worldPos;
}