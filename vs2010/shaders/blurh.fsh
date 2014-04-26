#define TYPE trans_alphach_ref

vec2 offsetArray[7];

void main() 
{
	offsetArray[0] = vec2(0.0, 0.0);
	offsetArray[1] = vec2(-1.5 * xi_inverseResolution.x, 0.0);
	offsetArray[2] = vec2(1.5 * xi_inverseResolution.x, 0.0);
	offsetArray[3] = vec2(-2.5 * xi_inverseResolution.x, 0.0);
	offsetArray[4] = vec2(2.5 * xi_inverseResolution.x, 0.0);
	offsetArray[5] = vec2(-4.5 * xi_inverseResolution.x, 0.0);
	offsetArray[6] = vec2(4.5 * xi_inverseResolution.x, 0.0);

	vec4 BlurCol = vec4(0.0, 0.0, 0.0, 0.0);

	for( int ii = 0; ii < 7; ++ii ) {
		BlurCol += texture2D( xi_textureSample0, clamp( gl_TexCoord[0].xy + offsetArray[ii] * 3.0, vec2( 0.01, 0.001 ), vec2( 0.999, 0.999 ) ) );
	}
	
	BlurCol /= 7.0;
	
	gl_FragColor = BlurCol;
}