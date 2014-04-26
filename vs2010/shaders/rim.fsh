#define TYPE solid

#define COUNT 9

vec2 offsetArray[COUNT];

void main() {
	vec4 depth = texture2D( xi_textureSample1, xi_textureUV0 );
	if ( depth.r == 0.0 ) {
		gl_FragColor = texture2D( xi_textureSample0, xi_textureUV0 );
		return;
	}
	
	offsetArray[0] = vec2(1.0 * xi_inverseResolution.x, 0.0);
	offsetArray[1] = vec2(2.0 * xi_inverseResolution.x, 0.0);
	offsetArray[2] = vec2(3.0 * xi_inverseResolution.x, 0.0);
	offsetArray[3] = vec2(-1.0 * xi_inverseResolution.x, 0.0);
	offsetArray[4] = vec2(-2.0 * xi_inverseResolution.x, 0.0);
	offsetArray[5] = vec2(-3.0 * xi_inverseResolution.x, 0.0);
	offsetArray[6] = vec2( 0.0, 1.0 * xi_inverseResolution.y );
	offsetArray[7] = vec2( 0.0, 2.0 * xi_inverseResolution.y );
	offsetArray[8] = vec2( 0.0, 3.0 * xi_inverseResolution.y );
	
	float depthTest[COUNT];
	for ( int ii = 0; ii < COUNT; ii++ ) {
		depthTest[ii] = texture2D( xi_textureSample1, xi_textureUV0 + offsetArray[ii] ).r;
		
		if ( depthTest[ii] == 0.0 ) {
			gl_FragColor = texture2D( xi_textureSample0, xi_textureUV0 ) * vec4( 2.0, 2.0, 2.0, 1.0 );
			return;
		}
	}
	
	discard;
}
