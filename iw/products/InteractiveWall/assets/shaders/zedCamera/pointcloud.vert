#version 330 core

in vec4 in_VertexRGBA;
uniform mat4 ciModelViewProjection;
uniform float leftRightClamping;
out vec3 b_color;

vec4 decomposeFloat(const in float value) {
	uint rgbaInt = floatBitsToUint(value);
	uint bIntValue = (rgbaInt / 256U / 256U) % 256U;
   	uint gIntValue = (rgbaInt / 256U) % 256U;
  	uint rIntValue = (rgbaInt) % 256U; 
 	return vec4(rIntValue / 255.0f, gIntValue / 255.0f, bIntValue / 255.0f, 1.0); 
}

void main(void) {
	// Decompose the 4th channel of the XYZRGBA buffer to retrieve the color of the point (1float to 4uint)
    b_color = decomposeFloat(in_VertexRGBA.a).xyz;
	float a = 1;
	if (in_VertexRGBA.x < -leftRightClamping || in_VertexRGBA.x > leftRightClamping)
		a = 0;
    gl_Position = ciModelViewProjection * vec4(vec3(1. - in_VertexRGBA.x, in_VertexRGBA.y, in_VertexRGBA.z), a);
};
