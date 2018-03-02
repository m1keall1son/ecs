#version 150

uniform sampler2D uTexture;

in Varyings	{
	vec3 baricentric;
	vec2 texcoord;
} f;

out vec4 FragColor;

/*
float edgeFactorDashed()
{
    // one way to calculate interpolation factor
    float f = f.baricentric.x;
    if( f.baricentric.x < min(f.baricentric.y, f.baricentric.z) )
        f = f.baricentric.y;

    const float PI = 3.14159265;
    float stipple = pow( clamp( 5 * sin( f * 21 * PI ), 0, 1 ), 10 );
    float thickness = 2.0 * stipple;

    vec3 d = fwidth(f.baricentric);
    vec3 a3 = smoothstep(vec3(0.0), d * thickness, f.baricentric);
    return min(min(a3.x, a3.y), a3.z);
}
*/

float edgeFactor()
{
	vec3 d = fwidth( f.baricentric );
	vec3 a3 = smoothstep( vec3(0.0), d * 1.5, f.baricentric );
	return min(min(a3.x, a3.y), a3.z);
}

void main(void) {
	// determine frag distance to closest edge
	float fEdgeIntensity = 1.0 - edgeFactor();

	// blend between edge color and face color
	vec4 vFaceColor = vec4( texture( uTexture, f.texcoord ).rgb, 0.25);
	vec4 vEdgeColor = vec4(1.0, 1.0, 0.0, 0.75);
	FragColor = mix(vFaceColor, vEdgeColor, fEdgeIntensity);
}