#version 430

out vec4 FragColor;

in Varyings {
	vec4 color;
} v;

void main()
{
	FragColor = v.color;
}