#version 430

uniform sampler2DArray uTextures;
const float uPremultiply = 0.;
const float uGamma = 2.2;

in Varyings	{
	vec2 texcoord;
	vec4 color;
	flat int texture_id;
} v;

out vec4 FragColor;
		
void main(void) {
	vec4 tex_sample = texture(uTextures, vec3(v.texcoord.x ,v.texcoord.y, float(v.texture_id)));
	FragColor = v.color*vec4(tex_sample);
};