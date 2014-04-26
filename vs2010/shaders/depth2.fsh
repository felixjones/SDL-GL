#define TYPE solid

void main() {
	float alpha = texture2D( xi_textureSample0, gl_TexCoord[1] ).a;
	if ( alpha < 0.5 ) {
		discard;
	}
	
	vec4 vInfo = gl_TexCoord[0];
	float depth = vInfo.z / vInfo.x;
	gl_FragColor = vec4( depth, depth * depth, 0.0, 1.0 );
}