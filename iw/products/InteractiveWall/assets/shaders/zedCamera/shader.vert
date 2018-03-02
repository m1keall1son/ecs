uniform mat4	ciModelViewProjection;
//uniform mat4	rot;
in vec4			ciPosition;
in vec2			ciTexCoord0;
out vec2		vUv;

void main( void )
{
	vUv = ciTexCoord0;
	gl_Position = ciModelViewProjection * ciPosition;
}
