#version 330

in vec4 glFragment_colour;
in vec2 glFragment_uv;
in vec3 glFragment_normal;
in vec4 glFragment_position;

out vec4 frame_colour;

uniform sampler2D	textureSample;
uniform float		shade;

void main() {
	frame_colour = texture( textureSample, glFragment_uv );
	
	if ( frame_colour.a > 0.0 ) {
	
		frame_colour *= glFragment_colour;
		frame_colour.rgb *= 1.0 / ( glFragment_position.z / glFragment_position.w );
	
	} else {
		discard;
	}	
}