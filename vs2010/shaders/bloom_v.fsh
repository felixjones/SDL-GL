#define TYPE solid

#define COUNT	( 7 )
#define COUNT_F	( 7.0 )

#define ONE 	( 1.0 )
#define TWO 	( 3.0 )
#define THREE	( 5.0 )
//#define FOUR 	( 4.0 )
//#define FIVE	( 5.0 )

vec2 offsetArray[COUNT];

void main() 
{
	offsetArray[0] = vec2( 0.0, 0.0);
	offsetArray[1] = vec2( 0.0, -ONE * xi_inverseResolution.y );
	offsetArray[2] = vec2( 0.0, ONE * xi_inverseResolution.y );
	offsetArray[3] = vec2( 0.0, -TWO * xi_inverseResolution.y );
	offsetArray[4] = vec2( 0.0, TWO * xi_inverseResolution.y );
	offsetArray[5] = vec2( 0.0, -THREE * xi_inverseResolution.y );
	offsetArray[6] = vec2( 0.0, THREE * xi_inverseResolution.y );
	//offsetArray[7] = vec2( 0.0, -FOUR * xi_inverseResolution.y );
	//offsetArray[8] = vec2( 0.0, FOUR * xi_inverseResolution.y );
	//offsetArray[9] = vec2( 0.0, -FIVE * xi_inverseResolution.y );
	//offsetArray[10] = vec2( 0.0, FIVE * xi_inverseResolution.y );

	vec4 blurColour = vec4( 0.0, 0.0, 0.0, 0.0 );

	for( int ii = 0; ii < COUNT; ii++ ) {
		blurColour += texture2D( xi_textureSample2, clamp( gl_TexCoord[0].xy + offsetArray[ii] * 3.0, vec2( 0.001, 0.001 ), vec2( 0.999, 0.999 ) ) );
	}
	
	blurColour /= COUNT_F;
	
	blurColour.a = 1.0;
	
	gl_FragColor = blurColour;
}