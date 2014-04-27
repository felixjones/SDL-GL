#version 330

layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

out vec2 f_TexCoord;
out vec2 f_PostCoord;

struct window_s {
	vec2	size;
	vec2	invSize;
	float	ratio;
};

uniform window_s	window;
uniform vec2		bufferSize;

/*
====================
main

	Geometry shader entry point
====================
*/
void main() {
	vec2 buffRes = vec2( window.size.x / bufferSize.x, window.size.y / bufferSize.y );
	
    gl_Position = vec4( 1.0, 1.0, 0.5, 1.0 );
	f_PostCoord = vec2( 1.0, 1.0 );
    f_TexCoord = vec2( 1.0, 1.0 ) * buffRes;
    EmitVertex();

    gl_Position = vec4(-1.0, 1.0, 0.5, 1.0 );
	f_PostCoord = vec2( 0.0, 1.0 );
    f_TexCoord = vec2( 0.0, 1.0 ) * buffRes; 
    EmitVertex();

    gl_Position = vec4( 1.0,-1.0, 0.5, 1.0 );
	f_PostCoord = vec2( 1.0, 0.0 );
    f_TexCoord = vec2( 1.0, 0.0 ) * buffRes; 
    EmitVertex();

    gl_Position = vec4(-1.0,-1.0, 0.5, 1.0 );
	f_PostCoord = vec2( 0.0, 0.0 );
    f_TexCoord = vec2( 0.0, 0.0 ) * buffRes; 
    EmitVertex();

    EndPrimitive(); 
}