#version 330

in vec2 fragUV;
out vec4 fragColour;

uniform sampler2D textureSample;
 
void main() {
    fragColour = texture( textureSample, fragUV );
}