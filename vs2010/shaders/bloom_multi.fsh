#define TYPE solid

void main() {
	//gl_FragColor = texture2D( xi_textureSample1, xi_textureUV0 );
	//return;
	
	vec4 bloom = texture2D( xi_textureSample3, xi_textureUV0 ).rgba;
	vec4 colour = texture2D( xi_textureSample0, xi_textureUV0 );
	
	if ( bloom.rgb == vec3( 0.0 ) ) {
		gl_FragColor = colour;
		return;
	}
	
	gl_FragColor = vec4( colour.rgb + bloom.rgb, 1.0 );
}