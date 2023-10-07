#version 330 core
out vec4 FragColor;

in vec4 outColor;
in vec2 outUV;

uniform sampler2D outTexture0;
uniform sampler2D outTexture1;
uniform float mixFactor;

void main()
{
	FragColor = vec4(1.0);
}