#version 330

in vec4 glFragment_colour;
in vec2 glFragment_uv;

out vec4 frame_colour;

uniform sampler2D	textureSample;
uniform float		shade;

void main() {
	mat2 scaleMatrix = mat2( 1.1, 0.0,
                             0.0, 1.1 );
							   
	mat2 RotationMatrix = mat2( cos(-shade), -sin(-shade),
                                sin(-shade),  cos(-shade) );
	
	vec2 newUv = vec2( 0.5 );
	newUv = RotationMatrix * ( glFragment_uv - newUv );
	newUv = scaleMatrix * newUv;
	newUv += vec2( 0.5 );
	
	if ( newUv.x <= 1.0 && newUv.x >= 0.0 && newUv.y <= 1.0 && newUv.y >= 0.0 ) {
		
		frame_colour = texture( textureSample, newUv );
		
		if ( frame_colour.a > 0.0 ) {
		
			frame_colour.rgb *= 2.0;
			frame_colour *= glFragment_colour;
			
		} else {
			//frame_colour = vec4( vec3( 0.0 ), 1.0 );
			discard;
		}
	} else {
		//frame_colour = vec4( vec3( 0.0 ), 1.0 );
		discard;
	}
}