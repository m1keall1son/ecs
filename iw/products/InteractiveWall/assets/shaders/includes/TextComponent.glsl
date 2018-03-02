
struct TextComponent
{
	vec4 color;
	vec4 bounds;
	ivec4 font_data;
};

layout( std430, binding=TextComponent_binding ) buffer text_component_buffer
{
	TextComponent textComponents[];
};

