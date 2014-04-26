#define TYPE solid

#define COUNT	( 7 )
#define COUNT_F	( 7.0 )

#define ONE 	( 1.0 )
#define TWO 	( 2.0 )
#define THREE	( 3.0 )
//#define FOUR 	( 4.0 )
//#define FIVE	( 5.0 )

vec2 offsetArray[COUNT];

void main() 
{
	offsetArray[0] = vec2(0.0, 0.0);
	offsetArray[1] = vec2(-ONE * xi_inverseResolution.x, 0.0);
	offsetArray[2] = vec2(ONE * xi_inverseResolution.x, 0.0);
	offsetArray[3] = vec2(-TWO * xi_inverseResolution.x, 0.0);
	offsetArray[4] = vec2(TWO * xi_inverseResolution.x, 0.0);
	offsetArray[5] = vec2(-THREE * xi_inverseResolution.x, 0.0);
	offsetArray[6] = vec2(THREE * xi_inverseResolution.x, 0.0);
	//offsetArray[7] = vec2(-FOUR * xi_inverseResolution.x, 0.0);
	//offsetArray[8] = vec2(FOUR * xi_inverseResolution.x, 0.0);
	//offsetArray[9] = vec2(-FIVE * xi_inverseResolution.x, 0.0);
	//offsetArray[10] = vec2(FIVE * xi_inverseResolution.x, 0.0);

	vec4 BlurCol = vec4(0.0, 0.0, 0.0, 0.0);

	for(int i = 0;i < COUNT;++i)
		BlurCol += texture2D(xi_textureSample2, clamp(gl_TexCoord[0].xy + offsetArray[i] * 3.0, vec2(0.001, 0.001), vec2(0.999, 0.999)));
	
	BlurCol /= COUNT_F;
	
	gl_FragColor = BlurCol;
}