#include "Matrix.h"

#include "Maths.h"
#include <string.h>

#define MAT4_CAST( X )	( ( mat4_t * )X )

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
	
	memcpy( matrix, &resMat, sizeof( mat4_t ) );
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