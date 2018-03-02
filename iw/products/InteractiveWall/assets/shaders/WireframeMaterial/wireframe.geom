#version 150

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in Varyings {
	vec2 texcoord;
} v[];

out Varyings	{
	vec3 baricentric;
	vec2 texcoord;
} f;

void main(void)
{
	f.baricentric = vec3(1, 0, 0);
	f.texcoord = v[0].texcoord;
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	f.baricentric = vec3(0, 1, 0);
	f.texcoord = v[1].texcoord;
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	f.baricentric = vec3(0, 0, 1);
	f.texcoord = v[2].texcoord;
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}