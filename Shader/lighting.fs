#version 330 core

#define MAX_POINT_COUNT 4

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shinness;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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
uniform vec3 viewPos;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight[MAX_POINT_COUNT];
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	// vec3 emission = texture(material.emission, outUV).rgb;

	vec3 norm = normalize(outNormal);
	vec3 viewDir = normalize(viewPos - outWorldPos.xyz);

	vec3 result = vec3(0);
	result += CalcDirLight(dirLight, norm, viewDir);
	for (int i = 0; i < MAX_POINT_COUNT; ++i)
	{
		result += CalcPointLight(pointLight[i], norm, outWorldPos.xyz, viewDir);
	}
	result += CalcSpotLight(spotLight, norm, outWorldPos.xyz, viewDir);
	
	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shinness);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, outUV));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, outUV));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, outUV));

	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shinness);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, outUV));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, outUV));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, outUV));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(lightDir, normalize(normal)), 0);

	vec3 reflectDir = reflect(-lightDir, normalize(normal));
	float spec = pow(max(dot(reflectDir, viewDir), 0), material.shinness);

	float theta = dot(-lightDir, normalize(light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, outUV));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, outUV));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, outUV));

	ambient *= intensity;
	diffuse *= intensity;
	specular *= intensity;

	return ambient + diffuse + specular;
}