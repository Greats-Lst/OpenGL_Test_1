#version 330 core

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec4 outColor;
in vec2 outUV;

uniform sampler2D outTexture0;
uniform sampler2D outTexture1;
uniform float mixFactor;
uniform Light light;

void main()
{
	FragColor = vec4(1);
}