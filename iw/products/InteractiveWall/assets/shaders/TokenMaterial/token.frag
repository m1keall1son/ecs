#version 430

out vec4 FragColor;

in Varyings {
	vec2 texcoord;
} v;

uniform sampler2D uTexture;

void main()
{
	FragColor = texture(uTexture, v.texcoord);
}