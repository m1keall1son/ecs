#version 150
#extension GL_ARB_shading_language_420pack : enable

struct TextRenderItem
{
	vec4 color;
	vec4 bounds;
	ivec4 font_data;
};

layout (std140, binding = 0 ) uniform uTextRenderItems
{ 
  TextRenderItem textRenderItems[256];
};

layout (std140, binding = 1 ) uniform uTextMatrices
{ 
  mat4 textModelMatrices[256];
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


uniform mat4 ciModelViewProjection;

in vec4 ciPosition;
in uint ModelMatrixLookup;
in uint GlyphLookup;

void main()
{
	gl_Position = ciModelViewProjection * textModelMatrices[ModelMatrixLookup] * vec4( glyphData[GlyphLookup].glyph.xy + ciPosition.xy * glyphData[GlyphLookup].glyph.zw, 0., 1. );
}