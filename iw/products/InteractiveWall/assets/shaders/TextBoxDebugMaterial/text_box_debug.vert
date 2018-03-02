#version 430

#include "RenderSystem.glsl"
#include "TextComponent.glsl"
#include "TextBoxDebugMaterial.glsl"

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
	TextComponent component = textComponents[ textComponentLookups[ MaterialInstanceLookup ] ];
	v.color = vec4(1.,0.,0.,1.);
	gl_Position = ciModelViewProjection * modelMatricies[ModelMatrixLookup] * vec4( component.bounds.xy + ciPosition.xy * component.bounds.zw, 0. , 1. );
}