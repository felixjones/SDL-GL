#define TYPE solid

void main() {
	float alpha = texture2D( xi_textureSample0, xi_textureUV0 ).a;
	if ( alpha < 0.5 ) {
		discard;
		return;
	}
	
	gl_FragColor = texture2D( xi_textureSample1, xi_textureUV0 ) * gl_FrontMaterial.emission;
}