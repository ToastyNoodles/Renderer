#version 420 core
out vec4 FragColor;

layout (binding = 0) uniform samplerCube skybox;

in vec3 fTexCoord;

void main()
{
	vec3 envColor = textureLod(skybox, fTexCoord, 6).rgb;
	envColor = envColor / (envColor + vec3(1.0));
	envColor = clamp(pow(envColor, vec3(1.0/2.2)), 0.0, 1.0);
	FragColor = vec4(envColor, 1.0);
}