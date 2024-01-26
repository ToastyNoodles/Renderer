#version 330 core
out vec4 FragColor;

in vec3 f_pos;
in vec3 f_norm;
in vec2 f_uv;
in vec3 f_tan;
in vec3 f_bitan;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_specular;

vec3 lightPos = vec3(2.0, 4.0, 2.0); //relative to the model matrix is seems
vec3 lightColor = vec3(0.4, 0.4, 0.1);
vec3 ambientLight = vec3(0.3, 0.3, 0.3);

void main()
{
	vec3 norm = normalize(f_norm);
	vec3 lightDir = normalize(lightPos - f_pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambientLight + diffuse);
	FragColor = texture(texture_diffuse, f_uv) * vec4(result, 1.0);
}