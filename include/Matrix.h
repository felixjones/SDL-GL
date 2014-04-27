#ifndef __MATRIX_H__
#define __MATRIX_H__

#if defined( __cplusplus )
extern "C" {
#endif

#include "Vector.h"

#define MAT2	( 2 )
#define MAT3	( 3 )
#define MAT4	( 4 )

/*
================================================================================

	Matrix structures
	Built in the style of GLSL
	They all contain padding so sizeof == sizeof( float ) * 16
	Use Matrix_ToFloatPtr for uploading onto GPU!

================================================================================
*/
typedef struct mat4_s {
	vec4_t	x;
	vec4_t	y;
	vec4_t	z;
	vec4_t	w;
} mat4_t;

typedef struct mat3_s {
	vec3_t	x;
	vec3_t	y;
	vec3_t	z;

	struct {
		vec3_t	w;
	} _padding;
} mat3_t;

typedef struct mat2_s {
	vec2_t	x;
	vec2_t	y;

	struct {
		vec2_t	z;
		vec2_t	w;
	} _padding;
} mat2_t;

void	Matrix_Clear( void * const matrix );
void	Matrix_ToFloatPtr( const void * const matrix, float * const floatPtr, const size_t length );
void	Matrix_Mul( void * const matrix, const void * const left, const void * const right, const size_t length );
void	Matrix_Inverse( void * const matrix, const void * const input, const size_t length );
float	Matrix_Determinant( const void * const matrix, const size_t length );
void	Matrix_LookAt( void * const matrix, const void * const eyePos, const void * const target, const void * const up );
void	Matrix_Perspective( void * const matrix, const float fovDegree, const float ratio, const float near, const float far );
void	Matrix_Orthographic( void * const matrix, const float left, const float right, const float bottom, const float top, const float near, const float far );
void	Matrix_RotationDegree( void * const matrix, const void * const rotationVec, const size_t length );
void	Matrix_Identity( void * const matrix );
void	Matrix_Scale(  void * const matrix, const void * const vector, const size_t length );
void	Matrix_Translation(  void * const matrix, const void * const vector, const size_t length );

#if defined( __cplusplus )
}
#endif

#endif