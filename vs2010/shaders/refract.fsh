#version 330

in vec2 f_TexCoord;
in vec2 f_PostCoord;
out vec4 f_Colour;

uniform vec2		postSize;
uniform float		time;
uniform sampler2D	s_BackBuffer;
uniform sampler2D	s_Texture;

/*
====================
main

	Fragment shader entry point
====================
*/
void main() {
	vec2 refraction = texture( s_Texture, f_PostCoord + vec2( time * 0.1 ) ).rg;
	refraction = refraction - vec2( 0.5 );
	refraction = refraction * postSize * 2.0;
	vec3 refracted = texture( s_BackBuffer, f_TexCoord + refraction ).rgb;
	
	f_Colour = vec4( refracted, 1.0 );
}