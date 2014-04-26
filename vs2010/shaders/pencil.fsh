#define TYPE solid

#define COUNT 8

vec2 offsetArray[COUNT];

void main() {
	vec4 depth = texture2D( xi_textureSample1, xi_textureUV0 );
	
	if ( depth.r == 0.0 ) {
		discard;
	}
		
	offsetArray[0] = vec2(1.0 * xi_inverseResolution.x, 0.0);
	offsetArray[1] = vec2(-1.0 * xi_inverseResolution.x, 0.0);
	offsetArray[2] = vec2(0.0, 1.0 * xi_inverseResolution.x);
	offsetArray[3] = vec2(0.0, -1.0 * xi_inverseResolution.x);
	offsetArray[4] = vec2(2.0 * xi_inverseResolution.x, 0.0);
	offsetArray[5] = vec2(-2.0 * xi_inverseResolution.x, 0.0);
	offsetArray[6] = vec2(0.0, 2.0 * xi_inverseResolution.x);
	offsetArray[7] = vec2(0.0, -2.0 * xi_inverseResolution.x);
		
	float depthTest[COUNT];
	for ( int ii = 0; ii < COUNT; ii++ ) {
		vec2 testUV = xi_textureUV0 + offsetArray[ii];
		
		if ( testUV.x > 1.0 || testUV.x < 0.0 || testUV.y > 1.0 || testUV.y < 0.0 ) {
			discard;
		}
		
		depthTest[ii] = texture2D( xi_textureSample1, testUV ).r;
		
		if ( depthTest[ii] == 0.0 ) {
			gl_FragColor = texture2D( xi_textureSample0, xi_textureUV0 ) * vec4( 0.75, 0.5, 0.75, 1.0 );
			return;
		}
	}
	
	discard;
}
