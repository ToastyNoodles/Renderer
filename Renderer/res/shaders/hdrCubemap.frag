#version 330 core
out vec4 FragColor;
in vec3 WorldPos;

<<<<<<< HEAD
in vec3 fPosition;

uniform sampler2D hdrTexture;
=======
uniform sampler2D equirectangularMap;
>>>>>>> aa0264bb92dbf7adb98928243dc685fb088ad3c3

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
<<<<<<< HEAD
{
	vec2 uv = SampleSphericalMap(normalize(fPosition));
	vec3 color = texture(hdrTexture, uv).rgb;

	FragColor = vec4(color, 1.0);
=======
{		
    vec2 uv = SampleSphericalMap(normalize(WorldPos));
    vec3 color = texture(equirectangularMap, uv).rgb;
    
    FragColor = vec4(color, 1.0);
>>>>>>> aa0264bb92dbf7adb98928243dc685fb088ad3c3
}