#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shinness;
};

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
in vec3 outNormal;
in vec4 outWorldPos;

uniform sampler2D outTexture0;
uniform sampler2D outTexture1;
uniform float mixFactor;
// uniform vec3 lightPos;
// uniform vec3 objectColor;
// uniform vec3 lightColor;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
	vec3 ambient = material.ambient * light.ambient;

	vec3 frag_2_light = normalize(light.position - outWorldPos.xyz);
	float cos = max(dot(frag_2_light, normalize(outNormal)), 0);
	vec3 diffuse = cos * light.diffuse * material.diffuse;

	vec3 frag_2_view = normalize(viewPos - outWorldPos.xyz);
	vec3 reflect = reflect(-frag_2_light, normalize(outNormal));
	float cos_specular = pow(max(dot(reflect, frag_2_view), 0), material.shinness);
	vec3 specular = cos_specular * material.specular * light.specular;

	vec3 result = ambient + diffuse + specular;
	// result *= objectColor;
	FragColor = vec4(result, 1.0);
}