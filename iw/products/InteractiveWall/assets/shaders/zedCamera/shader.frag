uniform sampler2D texImage;
in vec2 vUv;
out vec4 oColor;

void main( void )
{
	vec4 color=texture(texImage, vec2(1.-vUv.x,1.-vUv.y));
	oColor = vec4(color.b, color.g, color.r, color.a);
}