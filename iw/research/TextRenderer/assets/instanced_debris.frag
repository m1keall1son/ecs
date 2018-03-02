#version 330

out vec4 FragColor;

in vec4 vPosition;
in vec3 vNormal;

void main()
{
	vec3 N = normalize( vNormal );
	vec3 L =  vec3(0,0,1);
	vec3 lambert = vec3(1.) * max( dot(N,L), 0.25 );
	FragColor.rgb = lambert;
	FragColor.a = 1.;
}