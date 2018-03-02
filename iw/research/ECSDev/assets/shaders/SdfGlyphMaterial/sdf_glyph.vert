#version 430

#include "TextComponent.glsl"
#include "SdfGlyphMaterial.glsl"
#include "RenderSystem.glsl"

uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

in vec4 ciPosition;
in vec3 ciNormal;
in vec2 ciTexCoord0;

in uint ModelMatrixLookup;
in uint MaterialInstanceLookup;

out Varyings	{
	flat vec4 text_bounds;
	vec3 normal;
	vec4 position;
	vec2 texcoord;
	vec4 color;
	flat int font;
	flat int clipX;
	flat int clipY;
	flat vec2 corner;
} v;

void main()
{

	v.text_bounds = textComponents[ glyphs[MaterialInstanceLookup].text_data.x ].bounds;
	v.position = vec4( glyphs[MaterialInstanceLookup].glyph.xy + ciPosition.xy * glyphs[MaterialInstanceLookup].glyph.zw, 0., 1. );
	v.corner = ciTexCoord0;
	v.clipX = glyphs[MaterialInstanceLookup].text_data.z;
	v.clipY = glyphs[MaterialInstanceLookup].text_data.w;
	v.texcoord.x = mix( glyphs[MaterialInstanceLookup].tc.x, glyphs[MaterialInstanceLookup].tc.x + glyphs[MaterialInstanceLookup].tc.z, ciTexCoord0.x );
	v.texcoord.y = mix( glyphs[MaterialInstanceLookup].tc.y, glyphs[MaterialInstanceLookup].tc.y + glyphs[MaterialInstanceLookup].tc.w, 1. - ciTexCoord0.y );
	v.color = textComponents[ glyphs[MaterialInstanceLookup].text_data.x ].color;
	v.font = textComponents[ glyphs[MaterialInstanceLookup].text_data.x ].font_data.x;
	v.normal = ciNormalMatrix * mat3( modelMatricies[ModelMatrixLookup] ) * ciNormal;

	gl_Position = ciModelViewProjection * modelMatricies[ModelMatrixLookup] * v.position;

}