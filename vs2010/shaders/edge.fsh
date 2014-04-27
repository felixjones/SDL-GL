#version 330

in vec2 f_TexCoord;
out vec4 f_Colour;

uniform vec2		pixelSize;
uniform sampler2D	s_BackBuffer;

vec2 offsetArray[8];

/*
====================
main

	Fragment shader entry point
====================
*/
void main() {
	offsetArray[0] = vec2( 0.0, pixelSize.y );
	offsetArray[1] = pixelSize;
	offsetArray[2] = vec2( pixelSize.x, 0.0 );
	offsetArray[3] = vec2( pixelSize.x, -pixelSize.y );
	offsetArray[4] = vec2( 0.0, -pixelSize.y );
	offsetArray[5] = -( pixelSize );
	offsetArray[6] = vec2( -pixelSize.x, 0.0 );
	offsetArray[7] = vec2( -pixelSize.x, pixelSize.y );

	vec4 sample[9];

	sample[8] = texture( s_BackBuffer, f_TexCoord );

	for ( int ii = 0; ii < 8; ii++ ) {
		sample[ii] = texture( s_BackBuffer, f_TexCoord + offsetArray[ii] );
	}

	vec4 horizEdge = sample[2] + ( 2.0 * sample[5] ) + sample[8] - ( sample[0] + ( 2.0 * sample[3] ) + sample[6] );

	vec4 vertEdge = sample[0] + ( 2.0 * sample[1] ) + sample[2] - ( sample[6] + ( 2.0 * sample[7] ) + sample[8] );

	vec3 col = sqrt( ( horizEdge.rgb * horizEdge.rgb ) + ( vertEdge.rgb * vertEdge.rgb ) );
	
	f_Colour = vec4( col, 1.0 );
}
