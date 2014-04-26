#define TYPE trans_alphach

void main() {
	vec4 colour = texture2D( xi_textureSample0, xi_textureUV0 ) * gl_FrontMaterial.emission;
	
	gl_FragColor = colour;
}