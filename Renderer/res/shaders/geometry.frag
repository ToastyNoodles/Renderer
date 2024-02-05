#version 420 core
out vec4 FragColor;

layout (location = 0) out vec4 colorTextureOut;
layout (location = 1) out vec4 normalTextureOut;
layout (location = 2) out vec4 rmaTextureOut;
layout (location = 3) out vec4 positionTextureOut;

layout (binding = 0) uniform sampler2D colorTexture;
layout (binding = 1) uniform sampler2D normalTexture;
layout (binding = 2) uniform sampler2D rmaTexture;

in vec4 f_pos;
in vec3 f_normal;
in vec2 f_uvs;

void main()
{
	vec4 color = texture(colorTexture, f_uvs);
	vec4 normal = texture(normalTexture, f_uvs);
	vec4 rma = texture(rmaTexture, f_uvs);

	colorTextureOut = color;
	normalTextureOut = normal;
	rmaTextureOut = rma;
	positionTextureOut = f_pos;
}