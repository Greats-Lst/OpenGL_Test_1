#version 330 core

struct Material
{
	// vec3 ambient;
	// vec3 diffuse;
	sampler2D diffuse;
	// vec3 specular;
	sampler2D specular;
	sampler2D emission;
	float shinness;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

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
uniform SpotLight spotLight;

void main()
{
	vec3 emission = texture(material.emission, outUV).rgb;

	vec3 frag_2_light = normalize(spotLight.position - outWorldPos.xyz);
	// ambient
	vec3 ambient = spotLight.ambient * texture(material.diffuse, outUV).rgb;
	// diffuse
	float cos = max(dot(frag_2_light, normalize(outNormal)), 0);
	vec3 diffuse = cos * spotLight.diffuse * texture(material.diffuse, outUV).rgb;
	// specular
	vec3 frag_2_view = normalize(viewPos - outWorldPos.xyz);
	vec3 reflect = reflect(-frag_2_light, normalize(outNormal));
	float cos_specular = pow(max(dot(reflect, frag_2_view), 0), material.shinness);
	vec3 specular = cos_specular * spotLight.specular * texture(material.specular, outUV).rgb;
	// final
	vec3 result;
	float theta = dot(-frag_2_light, spotLight.direction);
	float epsilon = spotLight.cutOff - spotLight.outerCutOff;
	float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0f, 1.0f);
	result = ambient + diffuse * intensity + specular * intensity;
	
	FragColor = vec4(result, 1.0);
}