#version 330

in vec2 f_TexCoord;
in vec2 f_PostCoord;
out vec4 f_Colour;

uniform sampler2D	s_BackBuffer;
uniform sampler2D	s_Texture;

/*
====================
main

	Fragment shader entry point
====================
*/
void main() {
	vec3 colour = texture( s_BackBuffer, f_TexCoord ).rgb;
	f_Colour = vec4( vec3( 1.0 ) - colour, 1.0 );
}