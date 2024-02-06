#version 420 core
layout (location = 0) out vec4 colorTextureOut;
layout (location = 1) out vec4 normalTextureOut;
layout (location = 2) out vec4 rmaTextureOut;
layout (location = 3) out vec4 positionTextureOut;

layout (binding = 0) uniform sampler2D colorTexture;
layout (binding = 1) uniform sampler2D normalTexture;
layout (binding = 2) uniform sampler2D rmaTexture;

uniform vec4 objectColor;

in vec4 f_worldPos;
in vec3 f_normal;
in vec2 f_uvs;

vec4 color;
vec4 normal;
vec4 rma;

void main()
{
	color = texture(colorTexture, f_uvs);
	normal = texture(normalTexture, f_uvs);
	rma = texture(rmaTexture, f_uvs);

	if (color.r != 0)
	{
		colorTextureOut = color;
	}
	else
	{
		colorTextureOut = objectColor;
	}

	normalTextureOut = normal;
	rmaTextureOut = rma;
	positionTextureOut = f_worldPos;
}