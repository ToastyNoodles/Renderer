#version 420 core
layout (location = 0) out vec4 FragColor;

layout (binding = 0) uniform samplerCube skybox;

in vec3 f_uvs;

void main()
{
	FragColor = texture(skybox, f_uvs);
}