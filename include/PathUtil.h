#ifndef __PATH_UTIL_H__
#define __PATH_UTIL_H__

#include <stdint.h>
#include <stddef.h>

namespace PathUtil {
	void	UnixSlashes( char * const cstring );
	void	ToLower( char * const cstring );
	void	DeletePath( char * const dest, const char * const source );
	void	DeletePathFromPath( char * const dest, const char * const source, const size_t pathCount );
	void	SafeStrCopy( char * const dest, const char * const source, const size_t maxLen );
	ptrdiff_t	FindNext( const char * const string, const char letter, const size_t start );
	void	SplitFileName( const char * const name, char * const path = nullptr, char * const fileName = nullptr, char * const extension = nullptr, const bool makeLower = false );
	bool	HasFileExtension( const char * const fileName, const char * const ext1, const char * const ext2 = nullptr );
}

#endif