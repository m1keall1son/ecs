#version 430

#include "RenderSystem.glsl"
#include "GlyphDebugMaterial.glsl"
#include "SdfGlyphMaterial.glsl"

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
	vec4 glyph_pos = vec4( glyphs[ glyphLookups[ MaterialInstanceLookup ] ].glyph.xy + ciPosition.xy * glyphs[glyphLookups[ MaterialInstanceLookup ] ].glyph.zw, 0., 1. );
	v.color = vec4(1.,0.,0.,1.);
	gl_Position = ciModelViewProjection * modelMatricies[ModelMatrixLookup] * glyph_pos;
}