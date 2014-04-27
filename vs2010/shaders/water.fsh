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
	vec2 texcoord = f_TexCoord;
	texcoord.x += sin( texcoord.y * 32.0 + time ) / 64.0;
  
	vec3 colour = texture( s_BackBuffer, texcoord ).rgb;
	
	f_Colour = vec4( colour, 1.0 );
}