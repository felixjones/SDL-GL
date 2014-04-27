#version 330

in vec2 f_TexCoord;
out vec4 f_Colour;

struct window_s {
	vec2	size;
	vec2	invSize;
	float	ratio;
};

uniform window_s	window;
uniform vec2		bufferSize;
uniform vec2		pixelSize;
uniform sampler2D	s_BackBuffer;

float FXAA_SPAN_MAX = 16.0;
float FXAA_REDUCE_MUL = 0.03125;
float FXAA_REDUCE_MIN = 0.0078125;

/*
====================
Fxaa

	FXAA edge anti-aliasing algorithm
====================
*/
vec3 Fxaa( sampler2D textureSampler, vec2 vertTexcoord, vec2 texcoordOffset ) {
	vec3 rgbNW = texture( textureSampler, vertTexcoord + ( vec2( -1.0, -1.0 ) * texcoordOffset ) ).rgb;
	vec3 rgbNE = texture( textureSampler, vertTexcoord + ( vec2( +1.0, -1.0 ) * texcoordOffset ) ).rgb;
	vec3 rgbSW = texture( textureSampler, vertTexcoord + ( vec2( -1.0, +1.0 ) * texcoordOffset ) ).rgb;
	vec3 rgbSE = texture( textureSampler, vertTexcoord + ( vec2( +1.0, +1.0 ) * texcoordOffset ) ).rgb;
	vec3 rgbM  = texture( textureSampler, vertTexcoord).rgb;

	vec3 luma = vec3( 0.299, 0.587, 0.114 );
	float lumaNW = dot( rgbNW, luma );
	float lumaNE = dot( rgbNE, luma );
	float lumaSW = dot( rgbSW, luma );
	float lumaSE = dot( rgbSE, luma );
	float lumaM  = dot( rgbM, luma );

	float lumaMin = min( lumaM, min( min( lumaNW, lumaNE ), min( lumaSW, lumaSE ) ) );
	float lumaMax = max( lumaM, max( max( lumaNW, lumaNE ), max( lumaSW, lumaSE ) ) );

	vec2 dir;
	dir.x = -( ( lumaNW + lumaNE ) - ( lumaSW + lumaSE ) );
	dir.y =  ( ( lumaNW + lumaSW ) - ( lumaNE + lumaSE ) );

	float dirReduce = max( ( lumaNW + lumaNE + lumaSW + lumaSE ) * FXAA_REDUCE_MUL, FXAA_REDUCE_MIN );

	float rcpDirMin = 1.0 / ( min( abs( dir.x ), abs( dir.y ) ) + dirReduce );

	dir = min( vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX ), 
	max( vec2( -FXAA_SPAN_MAX, -FXAA_SPAN_MAX ), dir * rcpDirMin ) ) * texcoordOffset;

	vec2 dir2 = dir * 0.5;
	vec3 rgbA = 0.5 * ( texture( textureSampler, vertTexcoord.xy + ( dir * -0.23333333 ) ).xyz + texture( textureSampler, vertTexcoord.xy + ( dir * 0.16666666 ) ).xyz);
	vec3 rgbB = ( rgbA * 0.5 ) + ( 0.25 * ( texture( textureSampler, vertTexcoord.xy - dir2 ).xyz + texture( textureSampler, vertTexcoord.xy + dir2 ).xyz ) );
	float lumaB = dot( rgbB, luma );

	if ( ( lumaB < lumaMin ) || ( lumaB > lumaMax ) ) {
		return rgbA;
	}

	return rgbB;
}

/*
====================
main

	Fragment shader entry point
====================
*/
void main() {
	vec3 antiAlias = Fxaa( s_BackBuffer, f_TexCoord, pixelSize );
	f_Colour = vec4( antiAlias, 1.0 );
}