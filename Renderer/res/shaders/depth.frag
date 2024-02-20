#version 420 core
out vec4 FragColor;

in vec2 f_uv;

uniform sampler2D depthTexture;

void main()
{
	float depthValue = texture(depthTexture, f_uv).r;
	FragColor = vec4(vec3(depthValue), 1.0);
}