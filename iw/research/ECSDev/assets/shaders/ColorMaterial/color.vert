#version 430

#include "RenderSystem.glsl"
#include "ColorMaterial.glsl"

in vec4 ciPosition;
in vec3 ciNormal;
in vec2 ciTexCoord0;

in uint ModelMatrixLookup;
in uint MaterialInstanceLookup;

out Varyings {
	vec4 color;
} v;

uniform mat4 ciModelViewProjection;

void main()
{
	v.color = colorMaterials[ MaterialInstanceLookup ];
	gl_Position = ciModelViewProjection * modelMatricies[ ModelMatrixLookup ] * ciPosition;
}