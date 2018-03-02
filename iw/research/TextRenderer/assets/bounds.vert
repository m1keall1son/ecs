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

uniform mat4 ciModelViewProjection;

in vec4 ciPosition;

void main()
{
	gl_Position = ciModelViewProjection * textModelMatrices[gl_InstanceID] * vec4( textRenderItems[gl_InstanceID].bounds.xy + ciPosition.xy * textRenderItems[gl_InstanceID].bounds.zw, 0., 1. );
}