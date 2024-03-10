#version 420 core
layout (location = 0) in vec3 aPosition;

out vec3 fTexCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	fTexCoord = aPosition;

	mat4 rotView = mat4(mat3(view)); //Removes translation from view matrix
	vec4 clipPos = projection * rotView * vec4(fTexCoord, 1.0);
	gl_Position = clipPos.xyww;
}