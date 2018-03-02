#version 430

out vec4 FragColor;

in Varyings {
	vec2 texcoord;
} v;

uniform sampler2DRect uVideo;
uniform float uAlpha;
uniform vec2 uVideoSize;

void main()
{
	FragColor = texture(uVideo, v.texcoord * uVideoSize );
	FragColor.a = uAlpha;
}