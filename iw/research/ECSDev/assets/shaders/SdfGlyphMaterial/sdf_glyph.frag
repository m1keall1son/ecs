#version 430

uniform sampler2DArray uFonts;

const float       uPremultiply = 0.;
const float       uGamma = 2.2;

in Varyings	{
	flat vec4 text_bounds;
	vec3 normal;
	vec4 position;
	vec2 texcoord;
	vec4 color;
	flat int font;
	flat int clipX;
	flat int clipY;
	flat vec2 corner;
} v;

out vec4          FragColor;
	
float median( float r, float g, float b ) {
	return max( min( r, g ), min( max( r, g ), b ) );
}
	
vec2 safeNormalize( in vec2 v ) {
	float len = length( v );
	len = ( len > 0.0 ) ? 1.0 / len : 0.0;
	return v * len;
}
	
void main(void) {

	//
	int horizontal_clip = int( mix( 0, int( v.position.x < v.text_bounds.x || v.position.x > v.text_bounds.x + v.text_bounds.z ), float( v.clipX ) ) );
	int vertical_clip =  int( mix( 0, int( v.position.y <  v.text_bounds.y ||  v.position.y > v.text_bounds.y + v.text_bounds.w ), float( v.clipY ) ) );
	if( horizontal_clip == 1 || vertical_clip == 1 )discard;

	// Convert normalized texcoords to absolute texcoords.
	vec2 uv = v.texcoord * textureSize( uFonts, 0 ).xy;
	// Calculate derivates
	vec2 Jdx = dFdx( uv );
	vec2 Jdy = dFdy( uv );
	// Sample SDF texture (3 channels).
	vec3 font_tex_sample = textureLod( uFonts, vec3(v.texcoord.x ,v.texcoord.y, float(v.font) ), 0 ).rgb;
	// Calculate signed distance (in texels).
	float sigDist = median( font_tex_sample.r, font_tex_sample.g, font_tex_sample.b ) - 0.5;
	// For proper anti-aliasing, we need to calculate signed distance in pixels. We do this using derivatives.
	vec2 gradDist = safeNormalize( vec2( dFdx( sigDist ), dFdy( sigDist ) ) );
	vec2 grad = vec2( gradDist.x * Jdx.x + gradDist.y * Jdy.x, gradDist.x * Jdx.y + gradDist.y * Jdy.y );
	// Apply anti-aliasing.
	const float kThickness = 0.125;
	const float kNormalization = kThickness * 0.5 * sqrt( 2.0 );
	float afwidth = min( kNormalization * length( grad ), 0.5 );
	float opacity = smoothstep( 0.0 - afwidth, 0.0 + afwidth, sigDist );
    // If enabled apply pre-multiplied alpha. Always apply gamma correction.
	FragColor = vec4( v.color.rgb , v.color.a * pow( v.color.a * opacity, 1.0 / uGamma ) );
	FragColor.rgb *= FragColor.a; //premultiplied alpha
	//FragColor.rgb = vColor.rgb; //mix( vColor.rgb, vColor.rgb * FragColor.a, uPremultiply );
	//FragColor = vec4( 1, 0, 0, 1 );
};