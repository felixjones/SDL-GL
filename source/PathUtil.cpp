#include "PathUtil.h"

#include <string.h>
#include "FileList.h"
#define __UNSAFE__
#include "os.h"

#define TO_LOWER	( 32 )
#define MIN( X, Y )	( X < Y ? X : Y )

void PathUtil::UnixSlashes( char * const cstring ) {
	size_t length = strlen( cstring );
	do {
		if ( cstring[length] == '\\' ) {
			cstring[length] = '/';
		}
	} while ( length-- );
}

void PathUtil::ToLower( char * const cstring ) {
	size_t length = strlen( cstring );
	do {
		if ( cstring[length] >= 'A' && cstring[length] <= 'Z' ) {
			cstring[length] += TO_LOWER;
		}
	} while ( length-- );
}

void PathUtil::DeletePath( char * const dest, const char * const source ) {
	ptrdiff_t lastSlash = strlen( source );
	while ( lastSlash-- && source[lastSlash] != '/' );
	
	ptrdiff_t lastBackSlash = strlen( source );
	while ( lastBackSlash-- && source[lastBackSlash] != '\\' );
	
	lastSlash = ( lastSlash > lastBackSlash ? lastSlash : lastBackSlash );
	
	SafeStrCopy( dest, &source[lastSlash + 1], NAME_LEN );
}

void PathUtil::DeletePathFromPath( char * const dest, const char * const source, const size_t pathCount ) {
	// delete path from filename
	ptrdiff_t ii = strlen( source );
	size_t pathCounter = pathCount;

	// search for path separator or beginning
	while ( ii >= 0 ) {
		if ( source[ii] == '/' || source[ii] == '\\' ) {
			if ( --pathCounter <= 0 ) {
				break;
			}
		}
		--ii;
	}

	if ( ii > 0 ) {
		dest[ ii + 1 ] = 0;
		do {
			dest[ii] = source[ii];
		} while ( ii-- );
	} else {
		dest[0] = 0;
	}
}

void PathUtil::SafeStrCopy( char * const dest, const char * const source, const size_t maxLen ) {
	if ( !source ) {
		dest[0] = 0;
		return;
	}

	const size_t sourceLen = strlen( source ) + 1;
	const size_t copyLen = MIN( sourceLen, maxLen );
	memcpy( &dest[0], &source[0], sizeof( char ) * copyLen );
}

ptrdiff_t PathUtil::FindNext( const char * const string, const char letter, const size_t start ) {
	const size_t stringLen = strlen( string );
	for ( size_t ii = start; ii < stringLen; ii++ ) {
		if ( string[ii] == letter ) {
			return ii;
		}
	}

	return -1;
}

void PathUtil::SplitFileName( const char * const name, char * const path, char * const fileName, char * const extension, const bool makeLower ) {
	ptrdiff_t ii = strlen( name );
	ptrdiff_t extpos = ii;
	const size_t nameLen = strlen( name );

	// search for path separator or beginning
	while ( ii >= 0 ) {
		if ( name[ii] == '.' ) {
			extpos = ii;
			if ( extension ) {
				strncat( extension, name + extpos + 1, nameLen - ( extpos + 1 ) );
				if ( makeLower ) {
					ToLower( extension );
				}
			}
		} else if ( name[ii] == '/' || name[ii] == '\\' ) {
			if ( fileName ) {
				strncat( fileName, name + ii + 1, extpos - ( ii + 1 ) );
				if ( makeLower ) {
					ToLower( fileName );
				}
			}
			if ( path ) {
				strncat( path, name, ii + 1 );
				if ( makeLower ) {
					ToLower( path );
					UnixSlashes( path );
				}
			}

			return;
		}

		ii--;
	}

	if ( fileName ) {
		strncat( fileName, name, extpos );
		if ( makeLower ) {
			ToLower( extension );
		}
	}
}

bool PathUtil::HasFileExtension( const char * const fileName, const char * const ext1, const char * const ext2 ) {
	bool hasExt = true;

	char filePath[PATH_LEN];
	SafeStrCopy( &filePath[0], fileName, PATH_LEN );
	ToLower( filePath );
	
	const size_t fileNameLen = strlen( fileName );
	size_t dotPos = fileNameLen;
	while ( fileName[dotPos] != '.' && dotPos > 0 ) {
		dotPos--;
	}

	if ( dotPos == 0 ) {
		return false;
	}
	const size_t extLen = fileNameLen - dotPos - 1;

	const size_t ext1Len = strlen( ext1 );

	if ( extLen == ext1Len ) {
		const size_t start = fileNameLen - ext1Len;

		size_t index = 0;
		for ( size_t ii = start; ii < fileNameLen; ii++ ) {
			if ( filePath[ii] != ext1[index++] ) {
				hasExt = false;
				break;
			}
		}
	} else {
		hasExt = false;
	}

	if ( ext2 && !hasExt ) {
		hasExt = true;

		const size_t ext2Len = strlen( ext2 );
		
		if ( extLen == ext2Len ) {
			const size_t start2 = fileNameLen - ext2Len;

			size_t index = 0;
			for ( size_t ii = start2; ii < fileNameLen; ii++ ) {
				if ( filePath[ii] != ext2[index++] ) {
					hasExt = false;
					break;
				}
			}
		} else {
			hasExt = false;
		}
	}

	return hasExt;
}