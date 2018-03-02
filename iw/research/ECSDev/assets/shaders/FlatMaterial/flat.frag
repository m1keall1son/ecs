#version 430

out vec4 FragColor;

in Varyings {
	vec3 normal;
} f;

void main()
{
	vec3 N = normalize( f.normal );
	vec3 L =  vec3(0,0,1);
	vec3 lambert = vec3( max( dot(N,L), 0.25 ) );
	FragColor.rgb = lambert;
	FragColor.a = 1.;
}