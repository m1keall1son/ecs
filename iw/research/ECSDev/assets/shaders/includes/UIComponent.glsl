
struct UIComponent
{
	vec2 size;
	vec2 center;
	uint is_active;
	uint index;
	float active_time;
	float pad;
};

layout( std430, binding=UIComponent_binding ) buffer ui_component_buffer
{
	UIComponent uiComponents[];
};

