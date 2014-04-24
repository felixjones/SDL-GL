#include "os.h"

#if defined( __WIN_API__ )

	#pragma warning( disable: 4103 )
	#pragma pack( push, packing )
	#pragma pack( 1 )

	#define PACK_STRUCT

#elif defined( __POSIX__ )
	// According to the original Irrlicht source this is based on:
	// Using pragma pack might work with earlier gcc versions already, but
	// it started to be necessary with gcc 4.7 on mingw unless compiled with -mno-ms-bitfields.
	// And I found some hints on the web that older gcc versions on the other hand had sometimes
	// trouble with pragma pack while they worked with __attribute__((packed)).

	#if ( __GNUC__ >= 4 ) && ( __GNUC_MINOR__ >= 7 )

		#pragma pack( push, packing )
		#pragma pack( 1 )
		#define PACK_STRUCT

	#else

		#define PACK_STRUCT	__attribute__( ( packed ) )

	#endif

#else

	#error "Compiler does not support packing"

#endif