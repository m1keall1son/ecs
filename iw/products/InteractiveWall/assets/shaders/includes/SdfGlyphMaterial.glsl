
struct GlyphData
{
	vec4 tc;
	vec4 glyph;
	ivec4 text_data;
};

layout( std430, binding=SdfGlyphMaterial_binding ) buffer sdf_glyph_material_buffer
{
	GlyphData glyphs[];
};

