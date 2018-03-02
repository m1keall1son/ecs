#version 430

#include "RenderSystem.glsl"
#include "LambertMaterial.glsl"

in vec4 ciPosition;
in vec3 ciNormal;
in vec2 ciTexCoord0;

in uint ModelMatrixLookup;
in uint MaterialInstanceLookup;

out Varyings {
	vec4 color;
	vec3 normal;
} v;

uniform mat4 ciModelViewProjection;
uniform mat4 ciModelView;
uniform mat3 ciNormalMatrix;

void main()
{
	v.color = lambertMaterials[ MaterialInstanceLookup ];

	vec4 model_pos = modelMatricies[ ModelMatrixLookup ] * ciPosition;

	//normal matrix, todo, this could be on the cpu and passed up
	v.normal =  ciNormalMatrix * mat3( modelMatricies[ ModelMatrixLookup ] ) * ciNormal;

	gl_Position = ciModelViewProjection * model_pos;
}