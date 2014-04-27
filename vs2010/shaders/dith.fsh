#version 330

in vec2 f_TexCoord;
out vec4 f_Colour;

uniform vec2		pixelSize;
uniform sampler2D	s_BackBuffer;

vec4 dither[4];
float dithDiv;

/*
====================
FindClosest

	All the big dithering calculations are done here
====================
*/
float FindClosest( int x, int y, float c0 ) {
	float limit = 0.0;
	if ( x < 4 ) {
		limit = ( dither[x][y] + 1.0 ) * dithDiv;
	}

	if ( c0 < limit ) {
		return 0.0;
	}
	
	return 1.0;
}

/*
====================
main

	Fragment shader entry point
====================
*/
void main() {
	dithDiv = 1.0 / 64.0;

	dither[0] = vec4( 1.0, 33.0, 9.0, 41.0 );
	dither[1] = vec4( 49.0, 17.0, 57.0, 25.0 );
	dither[2] = vec4( 13.0, 45.0, 5.0, 37.0 );
	dither[3] = vec4( 61.0, 29.0, 53.0, 21.0 );

	vec3 rgb = texture2D( s_BackBuffer, f_TexCoord ).rgb;
	
	vec2 xy = gl_FragCoord.xy;
	int x = int( mod( xy.x, 4.0 ) );
	int y = int( mod( xy.y, 4.0 ) );

	vec3 finalRGB;

	finalRGB.r = FindClosest( x, y, rgb.r );
	finalRGB.g = FindClosest( x, y, rgb.g );
	finalRGB.b = FindClosest( x, y, rgb.b );

	f_Colour = vec4( finalRGB, 1.0 );
}