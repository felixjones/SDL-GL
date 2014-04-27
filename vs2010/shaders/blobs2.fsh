#version 330

in vec2 f_TexCoord;
out vec4 f_Colour;

uniform vec2		pixelSize;
uniform sampler2D	s_BackBuffer;

/*
====================
Blobs2

	Checks the darkness of surrounding fragments
	Will return the darkest in a circle
	This produces a circular blob
====================
*/
vec3 Blobs2( sampler2D textureSampler, vec2 vertTexcoord, vec2 texcoordOffset ) {
	vec4 u  = texture( textureSampler, vertTexcoord + vec2( 0.0, -texcoordOffset.y ) );
    vec4 d  = texture( textureSampler, vertTexcoord + vec2( 0.0, texcoordOffset.y ) );
    vec4 l  = texture( textureSampler, vertTexcoord + vec2( -texcoordOffset.x, 0.0 ) );
    vec4 r  = texture( textureSampler, vertTexcoord + vec2( texcoordOffset.x, 0.0 ) );
    
    vec4 v1 = min( l, r );
    vec4 v2 = min( u, d );
    vec4 v3 = min( v1, v2 );

    vec4 ul = texture( textureSampler, vertTexcoord + vec2( -texcoordOffset.x, -texcoordOffset.y ) );
    vec4 dr = texture( textureSampler, vertTexcoord + vec2( texcoordOffset.x, texcoordOffset.y ) );
    vec4 dl = texture( textureSampler, vertTexcoord + vec2( -texcoordOffset.x, texcoordOffset.y ) );
    vec4 ur = texture( textureSampler, vertTexcoord + vec2( texcoordOffset.x, -texcoordOffset.y ) );

    vec4 v4 = min( ul, dr );
    vec4 v5 = min( ur, dl );
    vec4 v6 = min( v4, v5 );
    
    vec4 v7 = min( v3, v6 );
    
    vec4 c  = texture( textureSampler, vertTexcoord );
    vec4 color = min( c, v7 );
	
    return color.rgb;
}

/*
====================
main

	Fragment shader entry point
====================
*/
void main() {
	vec3 artsy = Blobs2( s_BackBuffer, f_TexCoord, pixelSize * vec2( 1.5 ) );
	
    f_Colour = vec4( artsy, 1.0 );
}

