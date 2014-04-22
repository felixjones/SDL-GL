#ifndef __OS_H__
#define __OS_H__

#if defined( _WIN32 ) || defined( _WIN64 ) || defined( WIN32 ) || defined( WIN64 )
	#define __WINDOWS__

	#if defined( _WIN64 ) || defined( WIN64 )
		#define __X64__
	#elif defined( _WIN32 ) || defined( WIN32 )
		#define __X32__
	#endif

	#if defined( __UNSAFE__ )
		#pragma warning( disable: 4996 )
	#endif
#endif

#if defined( __APPLE__ )
	#include <TargetConditionals.h>

	#if defined( TARGET_OS_MAC ) && defined( __LP64__ )
		#define __OS_X__

		#define __X64__
	#elif defined( TARGET_OS_IPHONE )
		#define __IOS__

		#if defined( __LP64__ )
			#define __X64__
		#elif !defined( __LP64__ )
			#define __X32__
		#endif
	#endif
#endif

#if defined( __linux__ )
	#define __LINUX__

	#if defined( __LP64__ ) || defined( _LP64 )
		#define __X64__
	#elif !defined( __LP64__ ) && !defined( _LP64 )
		#define __X32__
	#endif
#endif

#if defined( __LINUX__ ) || defined( __OS_X__ ) || defined( __IOS__ )
	#define __POSIX__
#elif defined( __WINDOWS__ )
	#define	__WIN_API__
#endif

#if defined( _DEBUG ) || defined( DEBUG )
	#define __DEBUG__
#elif !defined( _DEBUG ) && !defined( DEBUG )
	#define __RELEASE__
#endif

#if !defined( __POSIX__ ) && !defined( __WIN_API__ )
	#error	"Unknown platform"
#endif

#if !defined( __X32__ ) && !defined( __X64__ )
	#error	"Unknown architecture"
#endif

#endif
