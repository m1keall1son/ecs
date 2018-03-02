#version 430

out vec4 FragColor;

in Varyings {
	vec4 color;
	vec2 texcoord;
} v;

uniform sampler2D uTexture;

void main()
{
	FragColor = texture(uTexture, v.texcoord) * v.color;
}