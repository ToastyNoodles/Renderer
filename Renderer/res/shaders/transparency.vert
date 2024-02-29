#version 420 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 fWorldPos;
out vec3 fNormal;
out vec2 fTexCoord;

void main()
{
	mat4 normalMatrix = transpose(inverse(model));

	fWorldPos = model * vec4(aPosition, 1.0);
	fNormal = normalize(vec3(normalMatrix) * aNormal);
	fTexCoord = aTexCoord;

	gl_Position = projection * view * fWorldPos;
}