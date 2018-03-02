#version 330 core

in vec3 b_color;
out vec4 oColor;
uniform bool humanpresent;

void main(void) {
	oColor = vec4(b_color, 1);
}