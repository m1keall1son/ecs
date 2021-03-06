#version 430

#include "RenderSystem.glsl"

in vec4 ciPosition;
in vec2 ciTexCoord0;

in uint ModelMatrixLookup;

out Varyings {
	vec2 texcoord;
} v;

uniform mat4 ciModelViewProjection;

void main()
{
	v.texcoord = ciTexCoord0;
	gl_Position = ciModelViewProjection * modelMatricies[ ModelMatrixLookup ] * ciPosition;
}