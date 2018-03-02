#version 430

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in Varyings {
	vec4 position;
} v[];

out Varyings	{
	vec3 normal;
} f;

uniform mat3 ciNormalMatrix;

#include "RenderSystem.glsl"

void main(void)
{

	vec3 delta1 = v[1].position.xyz - v[0].position.xyz;
	vec3 delta2 = v[2].position.xyz - v[0].position.xyz;

	//already takes position into account, so i don't need the model matrix
	vec3 normal = ciNormalMatrix * normalize( cross( delta1, delta2 ) );

	f.normal = normal;
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	f.normal = normal;
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	f.normal = normal;
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}