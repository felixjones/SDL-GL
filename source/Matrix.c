#include "Matrix.h"

#include "Maths.h"
#include <string.h>

#define MAT4_CAST( X )	( ( mat4_t * )X )
#define PI_180			( 0.01745329251f )
#define PI_360			( 0.00872664625f )
#define _180_PI			( 57.2957795131f )
#define _360_PI			( 114.591559026f )

/*
====================
Matrix_Clear

	Calls memset on the given matrix
====================
*/
void Matrix_Clear( void * const matrix ) {
	memset( matrix, 0, sizeof( mat4_t ) );
}

/*
====================
Matrix_ToFloatPtr

	Copies the matrix into a float pointer
====================
*/
void Matrix_ToFloatPtr( const void * const matrix, float * const floatPtr, const size_t length ) {
	size_t index = length - 1;
	do {
		Vector_ToFloatPtr( ( &MAT4_CAST( matrix )->x + index ), &floatPtr[length * index], length );
	} while ( index-- );
}

/*
====================
Matrix_Mul

	Multiplies two matrices of the same size together
====================
*/
void Matrix_Mul( void * const matrix, const void * const left, const void * const right, const size_t length ) {
	const float * const leftFloat = ( const float * )left;
	const float * const rightFloat = ( const float * )right;
	mat4_t resMat;
	float * const resFloat = ( float * )&resMat;
	size_t ii = length - 1;

	Matrix_Clear( &resMat );

	do {
		size_t jj = length - 1;

		do {
			float sum = 0.0f;
			size_t kk = length - 1;
			
			do {
				sum += rightFloat[kk * MAT4 + ii] * leftFloat[jj * MAT4 + kk];
			} while ( kk-- );

			resFloat[jj * MAT4 + ii] = sum;
		} while ( jj-- );
	} while ( ii-- );
	
	if ( length >= MAT2 ) {
		MAT4_CAST( matrix )->x.x = resMat.x.x;
		MAT4_CAST( matrix )->x.y = resMat.x.y;
		MAT4_CAST( matrix )->y.x = resMat.y.x;
		MAT4_CAST( matrix )->y.y = resMat.y.y;
	}
	if ( length >= MAT3 ) {
		MAT4_CAST( matrix )->x.z = resMat.x.z;
		MAT4_CAST( matrix )->y.z = resMat.y.z;
		MAT4_CAST( matrix )->z.x = resMat.z.x;
		MAT4_CAST( matrix )->z.y = resMat.z.y;
		MAT4_CAST( matrix )->z.z = resMat.z.z;
	}
	if ( length >= MAT4 ) {
		MAT4_CAST( matrix )->x.w = resMat.x.w;
		MAT4_CAST( matrix )->y.w = resMat.y.w;
		MAT4_CAST( matrix )->z.w = resMat.z.w;
		MAT4_CAST( matrix )->w.x = resMat.w.x;
		MAT4_CAST( matrix )->w.y = resMat.w.y;
		MAT4_CAST( matrix )->w.z = resMat.w.z;
		MAT4_CAST( matrix )->w.w = resMat.w.w;
	}
}

/*
====================
Matrix_Determinant

	Returns the float determinant of a given matrix
	The length determins the determinant algorithm
====================
*/
float Matrix_Determinant( const void * const matrix, const size_t length ) {
	if ( length == VEC2 ) {

		return ( MAT4_CAST( matrix )->x.x * MAT4_CAST( matrix )->y.y - MAT4_CAST( matrix )->x.y * MAT4_CAST( matrix )->y.x );

	} else if ( length == VEC3 ) {

		const float a = MAT4_CAST( matrix )->x.x * ( MAT4_CAST( matrix )->y.y * MAT4_CAST( matrix )->z.z - MAT4_CAST( matrix )->z.y * MAT4_CAST( matrix )->y.z );
		const float b = MAT4_CAST( matrix )->y.x * ( MAT4_CAST( matrix )->x.y * MAT4_CAST( matrix )->z.z - MAT4_CAST( matrix )->z.y * MAT4_CAST( matrix )->x.z );
		const float c = MAT4_CAST( matrix )->z.x * ( MAT4_CAST( matrix )->x.y * MAT4_CAST( matrix )->y.z - MAT4_CAST( matrix )->y.y * MAT4_CAST( matrix )->x.z );

		return ( a - b + c );

	} else if ( length == VEC4 ) {

		const float subFactor00 = MAT4_CAST( matrix )->z.z * MAT4_CAST( matrix )->w.w - MAT4_CAST( matrix )->w.z * MAT4_CAST( matrix )->z.w;
		const float subFactor01 = MAT4_CAST( matrix )->z.y * MAT4_CAST( matrix )->w.w - MAT4_CAST( matrix )->w.y * MAT4_CAST( matrix )->z.w;
		const float subFactor02 = MAT4_CAST( matrix )->z.y * MAT4_CAST( matrix )->w.z - MAT4_CAST( matrix )->w.y * MAT4_CAST( matrix )->z.z;
		const float subFactor03 = MAT4_CAST( matrix )->z.x * MAT4_CAST( matrix )->w.w - MAT4_CAST( matrix )->w.x * MAT4_CAST( matrix )->z.w;
		const float subFactor04 = MAT4_CAST( matrix )->z.x * MAT4_CAST( matrix )->w.z - MAT4_CAST( matrix )->w.x * MAT4_CAST( matrix )->z.z;
		const float subFactor05 = MAT4_CAST( matrix )->z.x * MAT4_CAST( matrix )->w.y - MAT4_CAST( matrix )->w.x * MAT4_CAST( matrix )->z.y;

		vec4_t detCof = {
				+ ( MAT4_CAST( matrix )->y.y * subFactor00 - MAT4_CAST( matrix )->y.z * subFactor01 + MAT4_CAST( matrix )->y.w * subFactor02 ),
				- ( MAT4_CAST( matrix )->y.x * subFactor00 - MAT4_CAST( matrix )->y.z * subFactor03 + MAT4_CAST( matrix )->y.w * subFactor04 ),
				+ ( MAT4_CAST( matrix )->y.x * subFactor01 - MAT4_CAST( matrix )->y.y * subFactor03 + MAT4_CAST( matrix )->y.w * subFactor05 ),
				- ( MAT4_CAST( matrix )->y.x * subFactor02 - MAT4_CAST( matrix )->y.y * subFactor04 + MAT4_CAST( matrix )->y.z * subFactor05 )
			};

		return ( MAT4_CAST( matrix )->x.x * detCof.x + MAT4_CAST( matrix )->x.y * detCof.y + MAT4_CAST( matrix )->x.z * detCof.z + MAT4_CAST( matrix )->x.w * detCof.w );

	}

	return 0.0f;
}

/*
====================
Matrix_Inverse

	Inverts the input matrix into matrix
	The length determins the algorithm
====================
*/
void Matrix_Inverse( void * const matrix, const void * const input, const size_t length ) {
	mat4_t temp;

	if ( length < MAT4 ) {
		memcpy( &temp, input, sizeof( mat4_t ) );
	}

	if ( length == MAT2 ) {
		
		const float oneOverDeterminant = SAFE_DIV( 1.0f, Matrix_Determinant( &temp, length ) );
		const float xx = temp.x.x;
		const float yy = temp.y.y;

		MAT4_CAST( matrix )->x.x = yy * oneOverDeterminant;
		MAT4_CAST( matrix )->x.y = -temp.x.y * oneOverDeterminant;
		MAT4_CAST( matrix )->y.x = -temp.y.x * oneOverDeterminant;
		MAT4_CAST( matrix )->y.y = xx * oneOverDeterminant;
		
	} else if ( length == MAT3 ) {
		
		const float oneOverDeterminant = SAFE_DIV( 1.0f, Matrix_Determinant( &temp, length ) );

		MAT4_CAST( matrix )->x.x =  ( temp.y.y * temp.z.z - temp.z.y * temp.y.z ) * oneOverDeterminant;
		MAT4_CAST( matrix )->y.x = -( temp.y.x * temp.z.z - temp.z.x * temp.y.z ) * oneOverDeterminant;
		MAT4_CAST( matrix )->z.x =  ( temp.y.x * temp.z.y - temp.z.x * temp.y.y ) * oneOverDeterminant;
		MAT4_CAST( matrix )->x.y = -( temp.x.y * temp.z.z - temp.z.y * temp.x.z ) * oneOverDeterminant;
		MAT4_CAST( matrix )->y.y =  ( temp.x.x * temp.z.z - temp.z.x * temp.x.z ) * oneOverDeterminant;
		MAT4_CAST( matrix )->z.y = -( temp.x.x * temp.z.y - temp.z.x * temp.x.y ) * oneOverDeterminant;
		MAT4_CAST( matrix )->x.z =  ( temp.x.y * temp.y.z - temp.y.y * temp.x.z ) * oneOverDeterminant;
		MAT4_CAST( matrix )->y.z = -( temp.x.x * temp.y.z - temp.y.x * temp.x.z ) * oneOverDeterminant;
		MAT4_CAST( matrix )->z.z =  ( temp.x.x * temp.y.y - temp.y.x * temp.x.y ) * oneOverDeterminant;
		
	} else if ( length == MAT4 ) {

		const float * const m = ( const float * )input;
		float inv[16];
		float det;
		size_t ii = 15;

		inv[0] =    m[5]  * m[10] * m[15] - 
					m[5]  * m[11] * m[14] - 
					m[9]  * m[6]  * m[15] + 
					m[9]  * m[7]  * m[14] +
					m[13] * m[6]  * m[11] - 
					m[13] * m[7]  * m[10];

		inv[4] =   -m[4]  * m[10] * m[15] + 
					m[4]  * m[11] * m[14] + 
					m[8]  * m[6]  * m[15] - 
					m[8]  * m[7]  * m[14] - 
					m[12] * m[6]  * m[11] + 
					m[12] * m[7]  * m[10];

		inv[8] =    m[4]  * m[9] * m[15] - 
					m[4]  * m[11] * m[13] - 
					m[8]  * m[5] * m[15] + 
					m[8]  * m[7] * m[13] + 
					m[12] * m[5] * m[11] - 
					m[12] * m[7] * m[9];

		inv[12] =  -m[4]  * m[9] * m[14] + 
					m[4]  * m[10] * m[13] +
					m[8]  * m[5] * m[14] - 
					m[8]  * m[6] * m[13] - 
					m[12] * m[5] * m[10] + 
					m[12] * m[6] * m[9];

		inv[1] =   -m[1]  * m[10] * m[15] + 
					m[1]  * m[11] * m[14] + 
					m[9]  * m[2] * m[15] - 
					m[9]  * m[3] * m[14] - 
					m[13] * m[2] * m[11] + 
					m[13] * m[3] * m[10];

		inv[5] = m[0]  * m[10] * m[15] - 
					m[0]  * m[11] * m[14] - 
					m[8]  * m[2] * m[15] + 
					m[8]  * m[3] * m[14] + 
					m[12] * m[2] * m[11] - 
					m[12] * m[3] * m[10];

		inv[9] =   -m[0]  * m[9] * m[15] + 
					m[0]  * m[11] * m[13] + 
					m[8]  * m[1] * m[15] - 
					m[8]  * m[3] * m[13] - 
					m[12] * m[1] * m[11] + 
					m[12] * m[3] * m[9];

		inv[13] =   m[0]  * m[9] * m[14] - 
					m[0]  * m[10] * m[13] - 
					m[8]  * m[1] * m[14] + 
					m[8]  * m[2] * m[13] + 
					m[12] * m[1] * m[10] - 
					m[12] * m[2] * m[9];

		inv[2] =   m[1]  * m[6] * m[15] - 
					m[1]  * m[7] * m[14] - 
					m[5]  * m[2] * m[15] + 
					m[5]  * m[3] * m[14] + 
					m[13] * m[2] * m[7] - 
					m[13] * m[3] * m[6];

		inv[6] =   -m[0]  * m[6] * m[15] + 
					m[0]  * m[7] * m[14] + 
					m[4]  * m[2] * m[15] - 
					m[4]  * m[3] * m[14] - 
					m[12] * m[2] * m[7] + 
					m[12] * m[3] * m[6];

		inv[10] =   m[0]  * m[5] * m[15] - 
					m[0]  * m[7] * m[13] - 
					m[4]  * m[1] * m[15] + 
					m[4]  * m[3] * m[13] + 
					m[12] * m[1] * m[7] - 
					m[12] * m[3] * m[5];

		inv[14] =  -m[0]  * m[5] * m[14] + 
					m[0]  * m[6] * m[13] + 
					m[4]  * m[1] * m[14] - 
					m[4]  * m[2] * m[13] - 
					m[12] * m[1] * m[6] + 
					m[12] * m[2] * m[5];

		inv[3] =   -m[1] * m[6] * m[11] + 
					m[1] * m[7] * m[10] + 
					m[5] * m[2] * m[11] - 
					m[5] * m[3] * m[10] - 
					m[9] * m[2] * m[7] + 
					m[9] * m[3] * m[6];

		inv[7] =	m[0] * m[6] * m[11] - 
					m[0] * m[7] * m[10] - 
					m[4] * m[2] * m[11] + 
					m[4] * m[3] * m[10] + 
					m[8] * m[2] * m[7] - 
					m[8] * m[3] * m[6];

		inv[11] =  -m[0] * m[5] * m[11] + 
					m[0] * m[7] * m[9] + 
					m[4] * m[1] * m[11] - 
					m[4] * m[3] * m[9] - 
					m[8] * m[1] * m[7] + 
					m[8] * m[3] * m[5];

		inv[15] =	m[0] * m[5] * m[10] - 
					m[0] * m[6] * m[9] - 
					m[4] * m[1] * m[10] + 
					m[4] * m[2] * m[9] + 
					m[8] * m[1] * m[6] - 
					m[8] * m[2] * m[5];

		det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
		
		det = SAFE_DIV( 1.0f, det );

		do {
			( ( float * )matrix )[ii] = inv[ii] * det;
		} while ( ii-- );

	}
}

void Matrix_LookAt( void * const matrix, const void * const eyePos, const void * const target, const void * const up ) {
	vec3_t xaxis;
	vec3_t yaxis;
	vec3_t zaxis;
	vec3_t upVector;

	if ( up ) {
		memcpy( &upVector, up, sizeof( upVector ) );
	} else {
		upVector.x = 0.0f;
		upVector.y = 1.0f;
		upVector.z = 0.0f;
	}

	Vector_Sub( &zaxis, target, eyePos );
	Vector_Normalise( &zaxis, &zaxis, VEC3 );

	Vector_Cross( &xaxis, &upVector, &zaxis );
	Vector_Normalise( &xaxis, &xaxis, VEC3 );
	
	Vector_Cross( &yaxis, &zaxis, &xaxis );

	MAT4_CAST( matrix )->x.x = xaxis.x;
	MAT4_CAST( matrix )->x.y = yaxis.x;
	MAT4_CAST( matrix )->x.z = zaxis.x;
	MAT4_CAST( matrix )->x.w = 0.0f;
	
	MAT4_CAST( matrix )->y.x = xaxis.y;
	MAT4_CAST( matrix )->y.y = yaxis.y;
	MAT4_CAST( matrix )->y.z = zaxis.y;
	MAT4_CAST( matrix )->y.w = 0.0f;
	
	MAT4_CAST( matrix )->z.x = xaxis.z;
	MAT4_CAST( matrix )->z.y = yaxis.z;
	MAT4_CAST( matrix )->z.z = zaxis.z;
	MAT4_CAST( matrix )->z.w = 0.0f;
	
	MAT4_CAST( matrix )->w.x = -Vector_Dot( &xaxis, eyePos, VEC3 );
	MAT4_CAST( matrix )->w.y = -Vector_Dot( &yaxis, eyePos, VEC3 );
	MAT4_CAST( matrix )->w.z = -Vector_Dot( &zaxis, eyePos, VEC3 );
	MAT4_CAST( matrix )->w.w = 1.0f;
}

void Matrix_Perspective( void * const matrix, const float fovDegree, const float ratio, const float near, const float far ) {
	const float size = near * Maths_Tan( fovDegree * PI_360 );
	const float left = -size;
	const float right = size;
	const float bottom = -size / ratio;
	const float top = size / ratio;

	Matrix_Clear( matrix );
	
	MAT4_CAST( matrix )->x.x = 2.0f * near / ( right - left );
	MAT4_CAST( matrix )->y.y = 2.0f * near / ( top - bottom );
	MAT4_CAST( matrix )->z.z = far / ( far - near );
	MAT4_CAST( matrix )->z.w = 1.0f;
	
	MAT4_CAST( matrix )->w.z = near * far / ( near - far );
}

void Matrix_Orthographic( void * const matrix, const float left, const float right, const float bottom, const float top, const float near, const float far ) {
	Matrix_Clear( matrix );
	
	MAT4_CAST( matrix )->x.x = 2.0f / ( right - left );
	MAT4_CAST( matrix )->y.y = 2.0f / ( top - bottom );
	MAT4_CAST( matrix )->z.z = 1.0f / ( far - near );

	MAT4_CAST( matrix )->w.z = near / ( near - far );
	MAT4_CAST( matrix )->w.w = 1.0f;
}

void Matrix_RotationDegree( void * const matrix, const void * const rotationVec, const size_t length ) {
	const vec3_t * const rotation = ( const vec3_t * )rotationVec;
	const float xRad = DEG2RAD( rotation->x );
	
	const float cr = Maths_Cos( xRad );
	const float sr = Maths_Sin( xRad );
	float cp = 0.0f;
	float sp = 0.0f;
	float cy = 0.0f;
	float sy = 0.0f;

	if ( length >= VEC2 ) {
		const float yRad = DEG2RAD( rotation->y );
		cp = Maths_Cos( yRad );
		sp = Maths_Sin( yRad );
	}

	if ( length >= VEC3 ) {
		const float zRad = DEG2RAD( rotation->z );
		cy = Maths_Cos( DEG2RAD( zRad ) );
		sy = Maths_Sin( DEG2RAD( zRad ) );
	}

	{
		const float srsp = sr * sp;
		const float crsp = cr * sp;

		MAT4_CAST( matrix )->x.x = cp * cy;
		MAT4_CAST( matrix )->x.y = cp * sy;
		MAT4_CAST( matrix )->x.z = -sp;

		MAT4_CAST( matrix )->y.x = srsp * cy - cr * sy;
		MAT4_CAST( matrix )->y.y = srsp * sy + cr * cy;
		MAT4_CAST( matrix )->y.z = sr * cp;

		MAT4_CAST( matrix )->z.x = crsp * cy + sr * sy;
		MAT4_CAST( matrix )->z.y = crsp * sy - sr * cy;
		MAT4_CAST( matrix )->z.z = cr * cp;
	}
}

void Matrix_Identity( void * const matrix ) {
	Matrix_Clear( matrix );
	
	MAT4_CAST( matrix )->x.x = 1.0f;
	MAT4_CAST( matrix )->y.y = 1.0f;
	MAT4_CAST( matrix )->z.z = 1.0f;
	MAT4_CAST( matrix )->w.w = 1.0f;
}

void Matrix_Scale(  void * const matrix, const void * const vector, const size_t length ) {
	const vec3_t * const vecPtr = ( const vec3_t * )vector;

	Matrix_Clear( matrix );
		
	MAT4_CAST( matrix )->w.w = 1.0f;

	MAT4_CAST( matrix )->x.x = vecPtr->x;

	if ( length >= MAT2 ) {
		MAT4_CAST( matrix )->y.y = vecPtr->y;
	} else {
		MAT4_CAST( matrix )->y.y = 1.0f;
	}
	
	if ( length >= MAT3 ) {
		MAT4_CAST( matrix )->z.z = vecPtr->z;
	} else {
		MAT4_CAST( matrix )->z.z = 1.0f;
	}
}

void Matrix_Translation(  void * const matrix, const void * const vector, const size_t length ) {
	const vec3_t * const vecPtr = ( const vec3_t * )vector;

	Matrix_Clear( matrix );
	
	MAT4_CAST( matrix )->x.x = 1.0f;
	MAT4_CAST( matrix )->y.y = 1.0f;
	MAT4_CAST( matrix )->z.z = 1.0f;
	MAT4_CAST( matrix )->w.w = 1.0f;

	if ( length == VEC2 ) {
		MAT4_CAST( matrix )->z.x = vecPtr->x;
		MAT4_CAST( matrix )->z.y = vecPtr->y;
	} else if ( length > VEC2 ) {
		MAT4_CAST( matrix )->w.x = vecPtr->x;
		MAT4_CAST( matrix )->w.y = vecPtr->y;
		MAT4_CAST( matrix )->w.z = vecPtr->z;
	}
}
