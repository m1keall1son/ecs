#version 430

#include "RenderSystem.glsl"
#include "TexturePoolMaterial.glsl"

uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

in vec4 ciPosition;
in vec3 ciNormal;
in vec2 ciTexCoord0;

in uint ModelMatrixLookup;
in uint MaterialInstanceLookup;

out Varyings {
	vec2 texcoord;
	vec4 color;
	flat int texture_id;
} v;

void main() {
	v.texcoord = ciTexCoord0;
	v.color = texture_pools[MaterialInstanceLookup].color;
	v.texture_id = texture_pools[MaterialInstanceLookup].texture_id.x;

	gl_Position = ciModelViewProjection * modelMatricies[ModelMatrixLookup] * ciPosition;
}