#include "Vector.h"

#include <string.h>
#include "Maths.h"

#define VEC4_CAST( X )	( ( vec4_t * )X )

/*
====================
Vector_Clear

	Calls memset on the vector
====================
*/
void Vector_Clear( void * const vector ) {
	memset( vector, 0, sizeof( vec4_t ) );
}

/*
====================
Vector_Add

	Adds two vectors together
====================
*/
void Vector_Add( void * const vector, const void * const left, const void * const right ) {
	VEC4_CAST( vector )->x = VEC4_CAST( left )->x + VEC4_CAST( right )->x;
	VEC4_CAST( vector )->y = VEC4_CAST( left )->y + VEC4_CAST( right )->y;
	VEC4_CAST( vector )->z = VEC4_CAST( left )->z + VEC4_CAST( right )->z;
	VEC4_CAST( vector )->w = VEC4_CAST( left )->w + VEC4_CAST( right )->w;
}

/*
====================
Vector_Sub

	Subtracts two vectors
====================
*/
void Vector_Sub( void * const vector, const void * const left, const void * const right ) {
	VEC4_CAST( vector )->x = VEC4_CAST( left )->x - VEC4_CAST( right )->x;
	VEC4_CAST( vector )->y = VEC4_CAST( left )->y - VEC4_CAST( right )->y;
	VEC4_CAST( vector )->z = VEC4_CAST( left )->z - VEC4_CAST( right )->z;
	VEC4_CAST( vector )->w = VEC4_CAST( left )->w - VEC4_CAST( right )->w;
}

/*
====================
Vector_Mul

	Multiplies two vectors
====================
*/
void Vector_Mul( void * const vector, const void * const left, const void * const right ) {
	VEC4_CAST( vector )->x = VEC4_CAST( left )->x * VEC4_CAST( right )->x;
	VEC4_CAST( vector )->y = VEC4_CAST( left )->y * VEC4_CAST( right )->y;
	VEC4_CAST( vector )->z = VEC4_CAST( left )->z * VEC4_CAST( right )->z;
	VEC4_CAST( vector )->w = VEC4_CAST( left )->w * VEC4_CAST( right )->w;
}

/*
====================
Vector_Div

	Divides two vectors
	There is a divide-by-zero fix that will return 0.0f
====================
*/
void Vector_Div( void * const vector, const void * const left, const void * const right ) {
	VEC4_CAST( vector )->x = SAFE_DIV( VEC4_CAST( left )->x, VEC4_CAST( right )->x );
	VEC4_CAST( vector )->y = SAFE_DIV( VEC4_CAST( left )->y, VEC4_CAST( right )->y );
	VEC4_CAST( vector )->z = SAFE_DIV( VEC4_CAST( left )->z, VEC4_CAST( right )->z );
	VEC4_CAST( vector )->w = SAFE_DIV( VEC4_CAST( left )->w, VEC4_CAST( right )->w );
}

/*
====================
Vector_MulMatrix

	Multiplies a vector with a given matrix of the same size
	Vec4 will multiply with Mat4, Vec3 with Mat3 and Vec2 with Mat2
====================
*/
void Vector_MulMatrix( void * const vector, const void * const input, const void * const matrix, const size_t length ) {
	const float * const vecFloat = ( const float * )input;
	const float * const matFloat = ( const float * )matrix;
	vec4_t resVec;
	float * const resFloat = ( float * )&resVec;
	size_t ii = length - 1;

	Vector_Clear( &resVec );

	do {
		float sum = 0.0f;
		size_t kk = length - 1;

		do {
			sum += matFloat[ii * VEC4 + kk] * vecFloat[kk];
		} while ( kk-- );

		resFloat[ii] = sum;
	} while ( ii-- );

	memcpy( vector, &resVec, sizeof( vec4_t ) );
}

/*
====================
Vector_ToFloatPtr

	Copies the vector into a float pointer
====================
*/
void Vector_ToFloatPtr( const void * const vector, float * const floatPtr, const size_t length ) {
	memcpy( floatPtr, vector, sizeof( float ) * length );
}

/*
====================
Vector_Min

	Returns the minimum between left and right vector for each element
====================
*/
void Vector_Min( void * const vector, const void * const left, const void * const right ) {
	VEC4_CAST( vector )->x = MIN( VEC4_CAST( left )->x, VEC4_CAST( right )->x );
	VEC4_CAST( vector )->y = MIN( VEC4_CAST( left )->y, VEC4_CAST( right )->y );
	VEC4_CAST( vector )->z = MIN( VEC4_CAST( left )->z, VEC4_CAST( right )->z );
	VEC4_CAST( vector )->w = MIN( VEC4_CAST( left )->w, VEC4_CAST( right )->w );
}

/*
====================
Vector_Max

	Returns the maximum between left and right vector for each element
====================
*/
void Vector_Max( void * const vector, const void * const left, const void * const right ) {
	VEC4_CAST( vector )->x = MAX( VEC4_CAST( left )->x, VEC4_CAST( right )->x );
	VEC4_CAST( vector )->y = MAX( VEC4_CAST( left )->y, VEC4_CAST( right )->y );
	VEC4_CAST( vector )->z = MAX( VEC4_CAST( left )->z, VEC4_CAST( right )->z );
	VEC4_CAST( vector )->w = MAX( VEC4_CAST( left )->w, VEC4_CAST( right )->w );
}

/*
====================
Vector_Clamp

	Clamps a given vector between min and max
====================
*/
void Vector_Clamp( void * const vector, const void * const input, const void * const min, const void * const max ) {
	Vector_Max( vector, input, min );
	Vector_Min( vector, input, max );
}

/*
====================
Vector_Mix

	Uses weighted averages between two vectors
====================
*/
void Vector_Mix( void * const vector, const void * const left, const void * const right, const void * const mixer ) {
	VEC4_CAST( vector )->x = AVG_WEIGHT( VEC4_CAST( left )->x, VEC4_CAST( right )->x, VEC4_CAST( mixer )->x );
	VEC4_CAST( vector )->y = AVG_WEIGHT( VEC4_CAST( left )->y, VEC4_CAST( right )->y, VEC4_CAST( mixer )->y );
	VEC4_CAST( vector )->z = AVG_WEIGHT( VEC4_CAST( left )->z, VEC4_CAST( right )->z, VEC4_CAST( mixer )->z );
	VEC4_CAST( vector )->w = AVG_WEIGHT( VEC4_CAST( left )->w, VEC4_CAST( right )->w, VEC4_CAST( mixer )->w );
}

/*
====================
Vector_MinFloat

	Returns the minimum between a vector and a float
	Applies to each element of the vector
====================
*/
void Vector_MinFloat( void * const vector, const void * const left, const float right ) {
	const vec4_t rightVec = { right, right, right, right };

	Vector_Min( vector, left, &rightVec );
}

/*
====================
Vector_MaxFloat

	Returns the maximum between a vector and a float
	Applies to each element of the vector
====================
*/
void Vector_MaxFloat( void * const vector, const void * const left, const float right ) {
	const vec4_t rightVec = { right, right, right, right };

	Vector_Max( vector, left, &rightVec );
}

/*
====================
Vector_ClampFloat

	Limits each element of the vector between min and max float
====================
*/
void Vector_ClampFloat( void * const vector, const void * const input, const float min, const float max ) {
	const vec4_t minVec = { min, min, min, min };
	const vec4_t maxVec = { max, max, max, max };

	Vector_Clamp( vector, input, &minVec, &maxVec );
}

/*
====================
Vector_MixFloat

	Uses weighted averages between left and right vector
	The float value is used on each element
====================
*/
void Vector_MixFloat( void * const vector, const void * const left, const void * const right, const float mixer ) {
	const vec4_t mixVec = { mixer, mixer, mixer, mixer };

	Vector_Mix( vector, left, right, &mixVec );
}

/*
====================
Vector_Cross

	Cross product between left and right vector
	vec4_t temp is used incase vector == left or == right
====================
*/
void Vector_Cross( void * const vector, const void * const left, const void * const right ) {
	vec4_t temp;

	temp.x = ( VEC4_CAST( left )->y * VEC4_CAST( right )->z ) - ( VEC4_CAST( left )->z * VEC4_CAST( right )->y );
	temp.y = ( VEC4_CAST( left )->z * VEC4_CAST( right )->x ) - ( VEC4_CAST( left )->x * VEC4_CAST( right )->z );
	temp.z = ( VEC4_CAST( left )->x * VEC4_CAST( right )->y ) - ( VEC4_CAST( left )->y * VEC4_CAST( right )->x );
	
	memcpy( vector, &temp, sizeof( vec4_t ) );
	VEC4_CAST( left )->w = 1.0f;
}

/*
====================
Vector_Dot

	Calculates the dot product of two vectors
====================
*/
float Vector_Dot( const void * const left, const void * const right, const size_t length ) {
	float val = ( VEC4_CAST( left )->x * VEC4_CAST( right )->x );

	val += ( length < VEC2 ? 0.0f : VEC4_CAST( left )->y * VEC4_CAST( right )->y );
	val += ( length < VEC3 ? 0.0f : VEC4_CAST( left )->z * VEC4_CAST( right )->z );
	val += ( length < VEC4 ? 0.0f : VEC4_CAST( left )->w * VEC4_CAST( right )->w );

	return val;
}

/*
====================
Vector_Length

	Calculates the length of a vector
====================
*/
float Vector_Length( const void * const vector, const size_t length ) {
	return Maths_Sqrt( Vector_Dot( vector, vector, length ) );
}

/*
====================
Vector_Distance

	Calculates the distance between two vectors
====================
*/
float Vector_Distance( const void * const left, const void * const right, const size_t length ) {
	vec4_t vector;

	Vector_Sub( &vector, left, right );

	return Vector_Length( &vector, length );
}

/*
====================
Vector_Normalise

	Normalises a vector
====================
*/
void Vector_Normalise( void * const vector, const void * const input, const size_t length ) {
	const float invLen = Maths_InvSqrt( Vector_Dot( input, input, length ) );
	
	VEC4_CAST( vector )->x = VEC4_CAST( input )->x * invLen;
	VEC4_CAST( vector )->y = ( length < VEC2 ? 0.0f : VEC4_CAST( input )->y * invLen );
	VEC4_CAST( vector )->z = ( length < VEC3 ? 0.0f : VEC4_CAST( input )->z * invLen );
	VEC4_CAST( vector )->w = ( length < VEC4 ? 0.0f : VEC4_CAST( input )->w * invLen );
}

/*
====================
Vector_FaceForward

	Returns a normal if a vertex's eye-space position points oposite to a given normal
	Returns the negated value otherwise
====================
*/
void Vector_FaceForward( void * const vector, const void * const n, const void * const i, const void * const nRef, const size_t length ) {
	if ( Vector_Dot( nRef, i, length ) < 0.0f ) {
		memcpy( vector, n, sizeof( float ) * length );
	} else {
		VEC4_CAST( vector )->x = -VEC4_CAST( n )->x;
		VEC4_CAST( vector )->y = ( length < VEC2 ? 0.0f : -VEC4_CAST( n )->y );
		VEC4_CAST( vector )->z = ( length < VEC2 ? 0.0f : -VEC4_CAST( n )->z );
		VEC4_CAST( vector )->w = ( length < VEC2 ? 0.0f : -VEC4_CAST( n )->w );
	}
}

/*
====================
Vector_Reflect

	Reflects vector i aginst the normal n
====================
*/
void Vector_Reflect( void * const vector, const void * const i, const void * const n, const size_t length ) {
	const float doubleDot = 2.0f * Vector_Dot( n, i, length );
	vec4_t doubleVec;

	doubleVec.x = VEC4_CAST( n )->x * doubleDot;
	doubleVec.y = ( length < VEC2 ? 0.0f : VEC4_CAST( n )->y * doubleDot );
	doubleVec.z = ( length < VEC3 ? 0.0f : VEC4_CAST( n )->z * doubleDot );
	doubleVec.w = ( length < VEC4 ? 0.0f : VEC4_CAST( n )->w * doubleDot );

	Vector_Sub( vector, i, &doubleVec );
}

/*
====================
Vector_Refract

	Complex physics of refraction!
	Vector i
	Normal n
	Rate of refraction r
====================
*/
void Vector_Refract( void * const vector, const void * const i, const void * const n, const float r, const size_t length ) {
	const float nDi = Vector_Dot( n, i, length );
	const float d = 1.0f - r * r * ( 1.0f - nDi * nDi );

	if ( d < 0.0f ) {
		Vector_Clear( vector );
	} else {
		const float rVS = ( r * nDi + Maths_Sqrt( d ) );
		vec4_t riVec;
		vec4_t rVSVec;

		riVec.x = VEC4_CAST( i )->x * r;
		riVec.y = ( length < VEC2 ? 0.0f : VEC4_CAST( i )->y * r );
		riVec.z = ( length < VEC3 ? 0.0f : VEC4_CAST( i )->z * r );
		riVec.w = ( length < VEC4 ? 0.0f : VEC4_CAST( i )->w * r );
		
		rVSVec.x = VEC4_CAST( n )->x * rVS;
		rVSVec.y = ( length < VEC2 ? 0.0f : VEC4_CAST( n )->y * rVS );
		rVSVec.z = ( length < VEC3 ? 0.0f : VEC4_CAST( n )->z * rVS );
		rVSVec.w = ( length < VEC4 ? 0.0f : VEC4_CAST( n )->w * rVS );

		Vector_Sub( vector, &riVec, &rVSVec );

		VEC4_CAST( vector )->x = ( riVec.x - rVSVec.x );
		VEC4_CAST( vector )->y = ( length < VEC2 ? 0.0f : riVec.y - rVSVec.y );
		VEC4_CAST( vector )->z = ( length < VEC3 ? 0.0f : riVec.z - rVSVec.z );
		VEC4_CAST( vector )->w = ( length < VEC4 ? 0.0f : riVec.w - rVSVec.w );
	}
}

/*
====================
Vector_Ceil

	Returns the ceil'd value of the float components
====================
*/
void Vector_Ceil( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = CEIL( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = CEIL( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = CEIL( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = CEIL( VEC4_CAST( input )->w );
}

/*
====================
Vector_Floor

	Returns the floor'd value of the float components
====================
*/
void Vector_Floor( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = FLOOR( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = FLOOR( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = FLOOR( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = FLOOR( VEC4_CAST( input )->w );
}

/*
====================
Vector_Round

	For each component, > 0.5f is ceil'd, else floor'd
====================
*/
void Vector_Round( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = ROUND( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = ROUND( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = ROUND( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = ROUND( VEC4_CAST( input )->w );
}

/*
====================
Vector_Radians

	Converts the components from degrees to radians
====================
*/
void Vector_Radians( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = DEG2RAD( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = DEG2RAD( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = DEG2RAD( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = DEG2RAD( VEC4_CAST( input )->w );
}

/*
====================
Vector_Degrees

	Converts the components from radians to degrees
====================
*/
void Vector_Degrees( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = RAD2DEG( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = RAD2DEG( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = RAD2DEG( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = RAD2DEG( VEC4_CAST( input )->w );
}

/*
====================
Vector_Sin

	Calls the trig function on all components
====================
*/
void Vector_Sin( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = Maths_Sin( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = Maths_Sin( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = Maths_Sin( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = Maths_Sin( VEC4_CAST( input )->w );
}

/*
====================
Vector_Cos

	Calls the trig function on all components
====================
*/
void Vector_Cos( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = Maths_Cos( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = Maths_Cos( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = Maths_Cos( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = Maths_Cos( VEC4_CAST( input )->w );
}

/*
====================
Vector_Tan

	Calls the trig function on all components
====================
*/
void Vector_Tan( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = Maths_Tan( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = Maths_Tan( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = Maths_Tan( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = Maths_Tan( VEC4_CAST( input )->w );
}

/*
====================
Vector_ASin

	Calls the trig function on all components
====================
*/
void Vector_ASin( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = Maths_ASin( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = Maths_ASin( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = Maths_ASin( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = Maths_ASin( VEC4_CAST( input )->w );
}

/*
====================
Vector_ACos

	Calls the trig function on all components
====================
*/
void Vector_ACos( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = Maths_ACos( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = Maths_ACos( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = Maths_ACos( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = Maths_ACos( VEC4_CAST( input )->w );
}

/*
====================
Vector_ATan

	Calls the trig function on all components
====================
*/
void Vector_ATan( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = Maths_ATan( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = Maths_ATan( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = Maths_ATan( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = Maths_ATan( VEC4_CAST( input )->w );
}

/*
====================
Vector_Pow

	The powerof vector contains the power
	The input vector contains the original numbers
	Outputs to vector
====================
*/
void Vector_Pow( void * const vector, const void * const input, const void * const powerof ) {
	VEC4_CAST( vector )->x = Maths_Pow( VEC4_CAST( input )->x, VEC4_CAST( powerof )->x );
	VEC4_CAST( vector )->y = Maths_Pow( VEC4_CAST( input )->y, VEC4_CAST( powerof )->y );
	VEC4_CAST( vector )->z = Maths_Pow( VEC4_CAST( input )->z, VEC4_CAST( powerof )->z );
	VEC4_CAST( vector )->w = Maths_Pow( VEC4_CAST( input )->w, VEC4_CAST( powerof )->w );
}

/*
====================
Vector_Exp

	Returns the exponent of each input element into the vector
====================
*/
void Vector_Exp( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = Maths_Exp( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = Maths_Exp( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = Maths_Exp( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = Maths_Exp( VEC4_CAST( input )->w );
}

/*
====================
Vector_FMA

	Fused multiply-add!
	Does ( a * b + c )
	Result goes in vector
====================
*/
void Vector_FMA( void * const vector, const void * const a, const void * const b, const void * const c ) {
	vec4_t temp;

	Vector_Mul( &temp, a, b );
	Vector_Add( vector, &temp, c );
}

/*
====================
Vector_Fract

	Gets the faction part of the input vector's elements
====================
*/
void Vector_Fract( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = FRACT( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = FRACT( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = FRACT( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = FRACT( VEC4_CAST( input )->w );
}

/*
====================
Vector_Equal

	Checks up to length elements of left and right
	Does a boolean comparison for them
====================
*/
int Vector_Equal( const void * const left, const void * const right, const size_t length ) {
	const int xEqual = !( VEC4_CAST( left )->x - VEC4_CAST( right )->x );
	const int yEqual = ( length >= VEC2 ? !( VEC4_CAST( left )->y - VEC4_CAST( right )->y ) : 1 );
	const int zEqual = ( length >= VEC3 ? !( VEC4_CAST( left )->z - VEC4_CAST( right )->z ) : 1 );
	const int wEqual = ( length >= VEC4 ? !( VEC4_CAST( left )->w - VEC4_CAST( right )->w ) : 1 );

	return ( xEqual && yEqual && zEqual && wEqual );
}

/*
====================
Vector_GreaterThan

	Checks up to length elements of left and right
	Does a boolean comparison for them
====================
*/
int Vector_GreaterThan( const void * const left, const void * const right, const size_t length ) {
	const int xEqual = ( VEC4_CAST( left )->x > VEC4_CAST( right )->x );
	const int yEqual = ( length >= VEC2 ? ( VEC4_CAST( left )->y > VEC4_CAST( right )->y ) : 1 );
	const int zEqual = ( length >= VEC3 ? ( VEC4_CAST( left )->z > VEC4_CAST( right )->z ) : 1 );
	const int wEqual = ( length >= VEC4 ? ( VEC4_CAST( left )->w > VEC4_CAST( right )->w ) : 1 );

	return ( xEqual && yEqual && zEqual && wEqual );
}

/*
====================
Vector_GreaterThanEqual

	Checks up to length elements of left and right
	Does a boolean comparison for them
====================
*/
int Vector_GreaterThanEqual( const void * const left, const void * const right, const size_t length ) {
	const int xEqual = ( VEC4_CAST( left )->x >= VEC4_CAST( right )->x );
	const int yEqual = ( length >= VEC2 ? ( VEC4_CAST( left )->y >= VEC4_CAST( right )->y ) : 1 );
	const int zEqual = ( length >= VEC3 ? ( VEC4_CAST( left )->z >= VEC4_CAST( right )->z ) : 1 );
	const int wEqual = ( length >= VEC4 ? ( VEC4_CAST( left )->w >= VEC4_CAST( right )->w ) : 1 );

	return ( xEqual && yEqual && zEqual && wEqual );
}

/*
====================
Vector_LessThan

	Checks up to length elements of left and right
	Does a boolean comparison for them
====================
*/
int Vector_LessThan( const void * const left, const void * const right, const size_t length ) {
	const int xEqual = ( VEC4_CAST( left )->x < VEC4_CAST( right )->x );
	const int yEqual = ( length >= VEC2 ? ( VEC4_CAST( left )->y < VEC4_CAST( right )->y ) : 1 );
	const int zEqual = ( length >= VEC3 ? ( VEC4_CAST( left )->z < VEC4_CAST( right )->z ) : 1 );
	const int wEqual = ( length >= VEC4 ? ( VEC4_CAST( left )->w < VEC4_CAST( right )->w ) : 1 );

	return ( xEqual && yEqual && zEqual && wEqual );
}

/*
====================
Vector_LessThanEqual

	Checks up to length elements of left and right
	Does a boolean comparison for them
====================
*/
int Vector_LessThanEqual( const void * const left, const void * const right, const size_t length ) {
	const int xEqual = ( VEC4_CAST( left )->x <= VEC4_CAST( right )->x );
	const int yEqual = ( length >= VEC2 ? ( VEC4_CAST( left )->y <= VEC4_CAST( right )->y ) : 1 );
	const int zEqual = ( length >= VEC3 ? ( VEC4_CAST( left )->z <= VEC4_CAST( right )->z ) : 1 );
	const int wEqual = ( length >= VEC4 ? ( VEC4_CAST( left )->w <= VEC4_CAST( right )->w ) : 1 );

	return ( xEqual && yEqual && zEqual && wEqual );
}

/*
====================
Vector_Sqrt

	Does sqrt on each element of input vector
====================
*/
void Vector_Sqrt( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = Maths_Sqrt( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = Maths_Sqrt( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = Maths_Sqrt( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = Maths_Sqrt( VEC4_CAST( input )->w );
}

/*
====================
Vector_InverseSqrt

	Does inverse sqrt on each element of input vector
====================
*/
void Vector_InverseSqrt( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = Maths_InvSqrt( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = Maths_InvSqrt( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = Maths_InvSqrt( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = Maths_InvSqrt( VEC4_CAST( input )->w );
}

/*
====================
Vector_Log

	Takes the log of each component of input vector
====================
*/
void Vector_Log( void * const vector, const void * const input ) {
	VEC4_CAST( vector )->x = Maths_Log( VEC4_CAST( input )->x );
	VEC4_CAST( vector )->y = Maths_Log( VEC4_CAST( input )->y );
	VEC4_CAST( vector )->z = Maths_Log( VEC4_CAST( input )->z );
	VEC4_CAST( vector )->w = Maths_Log( VEC4_CAST( input )->w );
}
