#version 430

#include "QuadTreeDebugMaterial.glsl"

in vec4 ciPosition;
in vec3 ciNormal;
in vec2 ciTexCoord0;

in uint ModelMatrixLookup;
in uint MaterialInstanceLookup;

out Varyings {
	vec4 color;
} v;

uniform mat4 ciProjectionMatrix;
uniform mat4 ciViewMatrix;
uniform ivec2 ciWindowSize;

void main()
{
	QuadTreeNode node = quadTreeDebugMaterials[ MaterialInstanceLookup ];
	v.color = mix( vec4( 1.,1.,0.,.05),  vec4( 1.,0.,0.,.5 ), clamp( float(node.quantity.x), 0.,1. ) );
	vec2 NDC = (node.area.xy + ( node.area.zw/2.f ) + ciPosition.xy * node.area.zw )/vec2(ciWindowSize);
	NDC.y = 1. - NDC.y;
	gl_Position =  vec4( NDC*2. - 1., 0. , 1. );
}