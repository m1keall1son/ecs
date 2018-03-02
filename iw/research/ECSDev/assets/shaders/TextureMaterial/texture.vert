#version 430

#include "RenderSystem.glsl"
#include "TextureMaterial.glsl"

in vec4 ciPosition;
in vec3 ciNormal;
in vec2 ciTexCoord0;

in uint ModelMatrixLookup;
in uint MaterialInstanceLookup;

out Varyings {
	vec4 color;
	vec2 texcoord;
} v;

uniform mat4 ciModelViewProjection;

void main()
{
	v.color = textureMaterials[ MaterialInstanceLookup ];
	v.texcoord = ciTexCoord0;
	gl_Position = ciModelViewProjection * modelMatricies[ ModelMatrixLookup ] * ciPosition;
}