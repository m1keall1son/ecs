#version 150

uniform mat4    ciViewMatrix;
uniform mat4    ciProjectionMatrix;
uniform mat4    ciModelMatrix;
uniform mat3	ciNormalMatrix;

uniform vec3    uLightPosition;

in vec4         ciPosition;
in vec3         ciNormal;
in vec3         pos;

out vec3		vNormal;
out vec3		vLightPosition;
out vec3		vPosition;


void main(){

	vec4 position           = vec4( ciPosition.xy*pos.z + pos.xy, ciPosition.z*pos.z, ciPosition.w );
    vec4 worldSpacePosition	= ciModelMatrix * position;
    vec4 viewSpacePosition	= ciViewMatrix * worldSpacePosition;
	
    vNormal					= ciNormalMatrix * ciNormal;
    vLightPosition			= ( ciViewMatrix * vec4( uLightPosition, 1.0 ) ).xyz;
    vPosition				= viewSpacePosition.xyz;
	
    gl_Position				= ciProjectionMatrix * viewSpacePosition;
}