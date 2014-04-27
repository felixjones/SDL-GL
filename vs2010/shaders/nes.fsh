#version 330

in vec2 f_TexCoord;
out vec4 f_Colour;

struct window_s {
	vec2	size;
	vec2	invSize;
	float	ratio;
};

uniform window_s	window;
uniform vec2		bufferSize;
uniform vec2		pixelSize;
uniform sampler2D	s_BackBuffer;

// 54 colours
// 8x8 colour patch
// 4 colours for each patch

/*
====================
ReduceRGB

	Reduces the number of colours of inRGB
	Count is how many colours per RGB component there is
====================
*/
vec3 ReduceRGB( vec3 inRGB, int count ) {
	float countF = float( count );

	vec3 outRGB = inRGB * countF;
	outRGB = floor( outRGB );

	return outRGB / countF;
}

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
	vec3 colour = texture( s_BackBuffer, Pixel( 4.0, f_TexCoord ) ).rgb;
	
	colour = colour * vec3( 2.0 );
	
	colour = ReduceRGB( colour, 4 );
	
	f_Colour = vec4( colour, 1.0 );
}