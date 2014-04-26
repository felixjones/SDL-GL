#define TYPE trans_alphach_ref

void main() {
	vec4 colour = texture2D( xi_textureSample0, xi_textureUV0 ) * gl_FrontMaterial.emission;
	
	if ( colour.a < 0.5 ) {
		discard;
	}
	
	gl_FragColor = colour;
}