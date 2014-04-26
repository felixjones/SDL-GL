#define TYPE solid

void main() {
	vec3 col = vec3( 1.0, 1.0, 1.0 ) - texture2D( xi_textureSample0, xi_textureUV0 ).rgb;
	gl_FragColor = vec4( col, 1.0 );
}