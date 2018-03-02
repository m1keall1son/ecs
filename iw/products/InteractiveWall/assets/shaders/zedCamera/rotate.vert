#version 330 core

in vec4 in_VertexRGBA;
uniform mat4 rot;
out vec4 rotated;

void main(void) {
	rotated = rot * vec4(vec3(in_VertexRGBA.x, in_VertexRGBA.y, in_VertexRGBA.z), 1);
};
