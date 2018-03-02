#version 430

out vec4 FragColor;

in Varyings {
	vec4 color;
	vec3 normal;
} v;

void main()
{
	vec3 N = normalize( v.normal );
	vec3 L =  vec3(0,0,1);
	vec3 lambert = v.color.rgb * max( dot(N,L), 0.25 );
	FragColor.rgb = lambert;
	FragColor.a = v.color.a;
}