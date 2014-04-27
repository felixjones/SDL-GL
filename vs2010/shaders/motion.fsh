#version 330

in vec2 f_TexCoord;
out vec4 f_Colour;

uniform vec2		bufferSize;
uniform vec2		pixelSize;
uniform sampler2D	s_BackBuffer;

/*
====================
Pixel

	Returns a fragment sample limited to fragSize
====================
*/
vec2 Pixel( float fragSize, vec2 texCoord ) {
	float dx = fragSize / bufferSize.x;
	float dy = fragSize / bufferSize.x;
	vec2 coord = vec2( dx * floor( texCoord.x / dx ), dy * floor( texCoord.y / dy ) );

	return coord;
}

/*
====================
main

	Fragment shader entry point
====================
*/
void main() {
	float blockSize = 2.0;
	vec2 offset = vec2( pixelSize.x * blockSize, 0.0 );

	f_Colour.r = texture( s_BackBuffer, Pixel( blockSize, f_TexCoord - offset ) ).r;
	f_Colour.g = texture( s_BackBuffer, Pixel( blockSize, f_TexCoord ) ).g;
	f_Colour.b = texture( s_BackBuffer, Pixel( blockSize, f_TexCoord + offset ) ).b;
	
	if ( int( f_TexCoord.y * bufferSize.y ) % 2 == 0 ) {
		f_Colour.rgb *= 0.5;
	}
	
	f_Colour.a = 0.5; // Alpha will give it the motion blur effect
}