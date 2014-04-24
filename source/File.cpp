#include "File.h"

#include <stdio.h>
#include <string.h>
#include "os.h"

xiFile::xiFile( const char * const _fileName ) {
	file = 0;
	fileSize = 0;
	references = 0;

	memset( &filename[0], 0, sizeof( char ) * FILE_NAME_LEN );
	
	const size_t fileNameLen = strlen( _fileName );
	const size_t copyLen = ( fileNameLen < FILE_NAME_LEN ? fileNameLen : FILE_NAME_LEN );

	memcpy( &filename[0], &_fileName[0], sizeof( char ) * copyLen );
}

xiFile::~xiFile() {
	if ( file ) {
		fclose( ( FILE * )file );
	}
}

void xiFile::Retain() {
	references++;
}

void xiFile::Release() {
	references--;
	if ( !references ) {
		delete( this );
	}
}

bool xiFile::Seek( const ptrdiff_t finalPos, const seekType_e seekType ) {
	if ( IsOpen() == false ) {
		return false;
	}

	size_t fseekPos = 0;

	if ( seekType == SEEK_RELATIVE ) {
#if defined( __WIN_API__ ) && defined ( __X64__ )
		fseekPos = _fseeki64( ( FILE * )file, finalPos, SEEK_CUR );
#else
		fseekPos = fseek( ( FILE * )file, ( int32_t )finalPos, SEEK_CUR );
#endif
	} else {
#if defined( __WIN_API__ ) && defined ( __X64__ )
		fseekPos = _fseeki64( ( FILE * )file, finalPos, SEEK_SET );
#else
		fseekPos = fseek( ( FILE * )file, ( int32_t )finalPos, SEEK_SET );
#endif
	}

	return ( fseekPos == 0 );
}

bool xiFile::IsOpen() const {
	return ( file != 0 );
}

size_t xiFile::GetPos() const {
#if defined( __WIN_API__ ) && defined ( __X64__ )
	return _ftelli64( ( FILE * )file );
#else
	return ftell( ( FILE * )file );
#endif
}

const char * xiFile::GetFileName() const {
	return &filename[0];
}
