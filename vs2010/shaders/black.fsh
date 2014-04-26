#define TYPE solid

void main() {
	float alpha = texture2D( xi_textureSample0, xi_textureUV0 ).a;
	if ( alpha < 0.5 ) {
		discard;
	}
	
	gl_FragColor = vec4( 0.0, 0.0, 0.0, 1.0 );
}