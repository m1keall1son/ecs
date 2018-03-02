#version 330

in vec4 ciPosition;
in vec3 ciNormal;
in mat4 modelMatrix;

uniform mat4 ciModelView;
uniform mat3 ciNormalMatrix;
uniform mat4 ciModelViewProjection;

out vec3 vNormal;
out vec4 vPosition;

void main()
{
	vNormal = ciNormalMatrix * ciNormal;
	vPosition = ciModelView * ciPosition;
	gl_Position = ciModelViewProjection * (modelMatrix * ciPosition);
}