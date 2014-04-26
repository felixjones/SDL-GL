#define TYPE solid

#define COUNT 6
#define COUNT_F 6.0

vec2 offsetArray[COUNT];

void main() {
	vec4 depth = texture2D( xi_textureSample1, xi_textureUV0 );
	
	if ( depth.r == 0.0 ) {
		discard;
	}
		
	offsetArray[0] = vec2(1.0 * xi_inverseResolution.x, 0.0);
	offsetArray[1] = vec2(2.0 * xi_inverseResolution.x, 0.0);
	offsetArray[2] = vec2(3.0 * xi_inverseResolution.x, 0.0);
	offsetArray[3] = vec2(4.0 * xi_inverseResolution.x, 0.0);
	offsetArray[4] = vec2(5.0 * xi_inverseResolution.x, 0.0);
	offsetArray[5] = vec2(6.0 * xi_inverseResolution.x, 0.0);
	
	float depthTest[COUNT];
	float thisDepth = 0.0;
	for ( int ii = 0; ii < COUNT; ii++ ) {
		vec2 testUV = xi_textureUV0 + offsetArray[ii];
		
		if ( testUV.x > 1.0 ) {
			discard;
		}
		
		depthTest[ii] = texture2D( xi_textureSample1, testUV ).r;
				
		if ( depthTest[ii] == 0.0 ) {
			gl_FragColor = texture2D( xi_textureSample0, xi_textureUV0 ) * vec4( 2.0, 2.0, 2.0, 1.0 );
			return;
		}
		
		thisDepth += depthTest[ii];
	}
	
	float diff = thisDepth / COUNT_F - depth.r;

	if ( diff > 0.005 ) {
		gl_FragColor = texture2D( xi_textureSample0, xi_textureUV0 ) * vec4( 2.0, 2.0, 2.0, 1.0 );
		return;
	}
	
	discard;
}
