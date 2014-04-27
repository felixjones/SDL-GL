#version 330

in vec2 f_TexCoord;
out vec4 f_Colour;

uniform vec2		pixelSize;
uniform sampler2D	s_BackBuffer;

vec2 offsetArray[11];
float sizeMod = 0.5;

/*
====================
main

	Horizontally blurs the input backBuffer
====================
*/
void main() {
	offsetArray[0] = vec2(0.0, 0.0);
	offsetArray[1] = vec2(-1.0 * pixelSize.x, 0.0) * vec2( sizeMod );
	offsetArray[2] = vec2(1.0 * pixelSize.x, 0.0) * vec2( sizeMod );
	offsetArray[3] = vec2(-2.0 * pixelSize.x, 0.0) * vec2( sizeMod );
	offsetArray[4] = vec2(2.0 * pixelSize.x, 0.0) * vec2( sizeMod );
	offsetArray[5] = vec2(-3.0 * pixelSize.x, 0.0) * vec2( sizeMod );
	offsetArray[6] = vec2(3.0 * pixelSize.x, 0.0) * vec2( sizeMod );
	offsetArray[7] = vec2(-4.0 * pixelSize.x, 0.0) * vec2( sizeMod );
	offsetArray[8] = vec2(4.0 * pixelSize.x, 0.0) * vec2( sizeMod );
	offsetArray[9] = vec2(-5.0 * pixelSize.x, 0.0) * vec2( sizeMod );
	offsetArray[10] = vec2(5.0 * pixelSize.x, 0.0) * vec2( sizeMod );

	vec4 blurCol = vec4(0.0, 0.0, 0.0, 0.0);

	for( int ii = 0; ii < 11; ++ii ) {
		blurCol += texture( s_BackBuffer, clamp( f_TexCoord + offsetArray[ii] * 3.0, vec2( 0.001, 0.001 ), vec2( 0.999, 0.999 ) ) );
	}
	
	blurCol /= 11.0;
	
	f_Colour = blurCol;
}