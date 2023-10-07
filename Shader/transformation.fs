#version 330 core
out vec4 FragColor;

in vec4 outColor;
in vec2 outUV;

uniform sampler2D outTexture0;
uniform sampler2D outTexture1;
uniform float mixFactor;

void main()
{
	vec4 color1 = texture(outTexture0, outUV);
	vec4 color2 = texture(outTexture1, outUV);
	FragColor =  mix(color1, color2, mixFactor);
}