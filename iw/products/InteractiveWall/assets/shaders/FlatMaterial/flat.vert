#version 430

#include "RenderSystem.glsl"

in vec4 ciPosition;
in vec2 ciTexCoord0;

in uint ModelMatrixLookup;

uniform mat4 ciModelViewProjection;

out Varyings {
	vec4 position;
} v;

void main()
{
	v.position = modelMatricies[ ModelMatrixLookup ] * ciPosition;
	gl_Position = ciModelViewProjection * v.position;
}