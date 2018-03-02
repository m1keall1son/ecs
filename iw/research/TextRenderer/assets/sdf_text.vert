#version 150
#extension GL_ARB_shading_language_420pack : enable

struct TextRenderItem
{
	vec4 color;
	vec4 bounds;
	ivec4 font_data;
};

layout (std140, binding = 0) uniform uTextRenderItems
{ 
  TextRenderItem textRenderItems[256];
};

struct GlyphData
{
	vec4 tc;
	vec4 glyph;
	ivec4 text_data;
};

layout (std140, binding = 2) uniform uGlyphData
{ 
  GlyphData glyphData[1024];
};

layout (std140, binding = 1) uniform uTextMatrices
{ 
  mat4 textModelMatrices[256];
};

uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

in vec4 ciPosition;
in vec3 ciNormal;
in vec2 ciTexCoord0;

in uint ModelMatrixLookup;
in uint GlyphLookup;

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

	v.text_bounds = textRenderItems[ glyphData[GlyphLookup].text_data.x ].bounds;
	v.position = vec4( glyphData[GlyphLookup].glyph.xy + ciPosition.xy * glyphData[GlyphLookup].glyph.zw, 0., 1. );
	v.corner = ciTexCoord0;
	v.clipX = glyphData[GlyphLookup].text_data.z;
	v.clipY = glyphData[GlyphLookup].text_data.w;
	v.texcoord.x = mix( glyphData[GlyphLookup].tc.x, glyphData[GlyphLookup].tc.x + glyphData[GlyphLookup].tc.z, ciTexCoord0.x );
	v.texcoord.y = mix( glyphData[GlyphLookup].tc.y, glyphData[GlyphLookup].tc.y + glyphData[GlyphLookup].tc.w, 1. - ciTexCoord0.y );
	v.color = textRenderItems[ glyphData[GlyphLookup].text_data.x ].color;
	v.font = textRenderItems[ glyphData[GlyphLookup].text_data.x ].font_data.x;
	v.normal = ciNormalMatrix * mat3( textModelMatrices[ModelMatrixLookup] ) * ciNormal;

	gl_Position = ciModelViewProjection * textModelMatrices[ModelMatrixLookup] * v.position;

}