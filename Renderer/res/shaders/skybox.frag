#version 420 core
out vec4 FragColor;

layout (binding = 0) uniform samplerCube skybox;

in vec3 fTexCoord;

void main()
{
	vec3 envColor = texture(skybox, fTexCoord).rgb;
	envColor = envColor / (envColor + vec3(1.0));
	envColor = pow(envColor, vec3(1.0/2.2));
	FragColor = vec4(envColor, 1.0);
}