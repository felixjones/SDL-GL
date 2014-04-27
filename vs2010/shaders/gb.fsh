#version 330

in vec2 f_TexCoord;
out vec4 f_Colour;

uniform vec2		bufferSize;
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
	vec3 colour = texture( s_BackBuffer, Pixel( 2.0, f_TexCoord ) ).rgb;
	
	colour = colour * colour;
	colour = colour * vec3( 4.0 );
	
	float avg = ( colour.r + colour.g + colour.b ) / 3.0;
	
	if ( avg < 0.25 ) {
		float rb = 15.0 / 255.0;
		colour = vec3( rb, 56.0 / 255.0, rb );
	} else if ( avg < 0.5 ) {
		float rb = 48.0 / 255.0;
		colour = vec3( rb, 98.0 / 255.0, rb );
	} else if ( avg < 0.75 ) {
		colour = vec3( 139.0 / 255.0, 172.0 / 255.0, 15.0 / 255.0 );
	} else {
		colour = vec3( 155.0 / 255.0, 188.0 / 255.0, 15.0 / 255.0 );
	}
	
	f_Colour = vec4( colour, 1.0 );
}