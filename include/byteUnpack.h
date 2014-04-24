#include "os.h"

#if defined( __WIN_API__ )

	#pragma pack( pop, packing )

#elif defined( __POSIX__ )

	#if ( __GNUC__ >= 4 ) && ( __GNUC_MINOR__ >= 7 )
		#pragma pack( pop, packing )
	#endif

#endif

#undef PACK_STRUCT