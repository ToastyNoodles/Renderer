#version 420 core
out vec4 FragColor;

layout (binding = 0) uniform samplerCube skybox;

in vec3 fTexCoord;

void main()
{
	FragColor = texture(skybox, fTexCoord);
}