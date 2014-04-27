#version 330

in vec4 glFragment_colour;
in vec2 glFragment_uv;

out vec4 frame_colour;

uniform sampler2D	textureSample;
uniform float		shade;

void main() {
	frame_colour = texture( textureSample, glFragment_uv );
	frame_colour *= glFragment_colour;
}