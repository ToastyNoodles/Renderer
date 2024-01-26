#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 f_pos;
out vec3 f_norm;
out vec2 f_uv;
out vec3 f_tan;
out vec3 f_bitan;

uniform mat4 viewProjection;
uniform mat4 model;

void main()
{
	f_pos = position;
	f_norm = normal;
	f_uv = uv;
	f_tan = tangent;
	f_bitan = bitangent;

	gl_Position = viewProjection * model * vec4(position, 1.0);
}