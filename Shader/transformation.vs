#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;

out vec4 outColor;
out vec2 outUV;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	outColor = vec4(aColor, 1.0f);
	outUV = aUV;
}