#include "ReadFile.h"

#include <stdio.h>
#include <string.h>
#define __UNSAFE__
#include "os.h"

xiReadFile * xiReadFile::CreateReadFile( const char * const _fileName ) {
	xiReadFile * newFile = new xiReadFile( _fileName );
	if ( newFile->IsOpen() ) {
		newFile->Retain();
		return newFile;
	}

	delete( newFile );
	return nullptr;
}

xiReadFile::xiReadFile( const char * const _fileName ) : xiFile( _fileName ) {
	OpenFile();
}

size_t xiReadFile::Read( void * const buffer, const size_t sizeToRead ) {
	if ( IsOpen() == false ) {
		return 0;
	}

	return fread( buffer, 1, sizeToRead, ( FILE * )file );
}

size_t xiReadFile::GetSize() const {
	return fileSize;
}

void xiReadFile::OpenFile() {
	if ( filename[0] == 0 ) {
		file = 0;
		return;
	}

	file = fopen( filename, "rb" );

	if ( file ) {
#if defined( __WIN_API__ ) && defined ( __X64__ )
		_fseeki64( ( FILE * )file, 0, SEEK_END );
		fileSize = GetPos();
		_fseeki64( ( FILE * )file, 0, SEEK_SET );
#else
		fseek( ( FILE * )file, 0, SEEK_END );
		fileSize = GetPos();
		fseek( ( FILE * )file, 0, SEEK_SET );
#endif
	}
}