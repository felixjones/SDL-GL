#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdint.h>

#define VEC2	( 2 )
#define VEC3	( 3 )
#define VEC4	( 4 )

/*
================================================================================

	Vector structures
	Built in the style of GLSL
	They all contain padding so sizeof == sizeof( float ) * 4
	Use Vector_ToFloatPtr for uploading onto GPU!

================================================================================
*/
typedef struct vec4_s {
	float	x;
	float	y;
	float	z;
	float	w;
} vec4_t;

typedef struct vec3_s {
	float	x;
	float	y;
	float	z;

	struct {
		float	w;
	} _padding;
} vec3_t;

typedef struct vec2_s {
	float	x;
	float	y;

	struct {
		float	z;
		float	w;
	} _padding;
} vec2_t;

void	Vector_Clear( void * const vector );
void	Vector_ToFloatPtr( const void * const vector, float * const floatPtr, const size_t length );
void	Vector_Add( void * const vector, const void * const left, const void * const right );
void	Vector_Sub( void * const vector, const void * const left, const void * const right );
void	Vector_Mul( void * const vector, const void * const left, const void * const right );
void	Vector_Div( void * const vector, const void * const left, const void * const right );
void	Vector_MulMatrix( void * const vector, const void * const input, const void * const matrix, const size_t length );
void	Vector_Min( void * const vector, const void * const left, const void * const right );
void	Vector_Max( void * const vector, const void * const left, const void * const right );
void	Vector_Clamp( void * const vector, const void * const input, const void * const min, const void * const max );
void	Vector_Mix( void * const vector, const void * const left, const void * const right, const void * const mixer );
void	Vector_MinFloat( void * const vector, const void * const left, const float right );
void	Vector_MaxFloat( void * const vector, const void * const left, const float right );
void	Vector_ClampFloat( void * const vector, const void * const input, const float min, const float max );
void	Vector_MixFloat( void * const vector, const void * const left, const void * const right, const float mixer );
void	Vector_Cross( void * const vector, const void * const left, const void * const right );
void	Vector_Normalise( void * const vector, const void * const input, const size_t length );
void	Vector_FaceForward( void * const vector, const void * const n, const void * const i, const void * const nRef, const size_t length );
void	Vector_Reflect( void * const vector, const void * const i, const void * const n, const size_t length );
void	Vector_Refract( void * const vector, const void * const i, const void * const n, const float r, const size_t length );
void	Vector_Ceil( void * const vector, const void * const input );
void	Vector_Floor( void * const vector, const void * const input );
void	Vector_Round( void * const vector, const void * const input );
void	Vector_Radians( void * const vector, const void * const input );
void	Vector_Sin( void * const vector, const void * const input );
void	Vector_Cos( void * const vector, const void * const input );
void	Vector_Tan( void * const vector, const void * const input );
void	Vector_ASin( void * const vector, const void * const input );
void	Vector_ACos( void * const vector, const void * const input );
void	Vector_ATan( void * const vector, const void * const input );
void	Vector_Pow( void * const vector, const void * const input, const void * const powerof );
void	Vector_Exp( void * const vector, const void * const input );
void	Vector_FMA( void * const vector, const void * const a, const void * const b, const void * const c );
void	Vector_Fract( void * const vector, const void * const input );
void	Vector_Sqrt( void * const vector, const void * const input );
void	Vector_InverseSqrt( void * const vector, const void * const input );
void	Vector_Log( void * const vector, const void * const input );
float	Vector_Dot( const void * const left, const void * const right, const size_t length );
float	Vector_Length( const void * const vector, const size_t length );
float	Vector_Distance( const void * const left, const void * const right, const size_t length );
int		Vector_Equal( const void * const left, const void * const right, const size_t length );
int		Vector_GreaterThan( const void * const left, const void * const right, const size_t length );
int		Vector_GreaterThanEqual( const void * const left, const void * const right, const size_t length );
int		Vector_LessThan( const void * const left, const void * const right, const size_t length );
int		Vector_LessThanEqual( const void * const left, const void * const right, const size_t length );

#endif