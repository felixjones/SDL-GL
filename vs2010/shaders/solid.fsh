#define TYPE solid

void main() {
	gl_FragColor = texture2D( xi_textureSample0, xi_textureUV0 ) * gl_FrontMaterial.emission;
}