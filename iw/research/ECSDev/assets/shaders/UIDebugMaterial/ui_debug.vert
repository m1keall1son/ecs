#version 430

#include "UIComponent.glsl"
#include "UIDebugMaterial.glsl"

in vec4 ciPosition;
in vec3 ciNormal;
in vec2 ciTexCoord0;

in uint ModelMatrixLookup;
in uint MaterialInstanceLookup;

out Varyings {
	vec4 color;
} v;

uniform ivec2 ciWindowSize;

void main()
{
	UIComponent component = uiComponents[ uiComponentLookups[ MaterialInstanceLookup ] ];
	v.color = mix( vec4( 0.,1.,0.,1.),  vec4( 1.,0.,1.,1. ), component.is_active );

	vec2 NDC = (component.center + ciPosition.xy * component.size)/vec2(ciWindowSize);
	NDC.y = 1. - NDC.y;
	gl_Position =  vec4( NDC*2. - 1., 0. , 1. );

}