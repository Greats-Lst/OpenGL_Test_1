#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;

out vec4 outColor;
out vec2 outUV;
out vec3 outNormal;
out vec4 outWorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	outColor = vec4(aColor, 1.0f);
	outUV = aUV;
	outNormal = mat3(transpose(inverse(model))) * aNormal;
	outWorldPos = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}