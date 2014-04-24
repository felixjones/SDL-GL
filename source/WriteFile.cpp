#include "WriteFile.h"

#include <stdio.h>
#define __UNSAFE__
#include "os.h"

xiWriteFile * xiWriteFile::CreateWriteFile( const char * const _fileName, const writeMode_e writeMode ) {
	xiWriteFile * newFile = new xiWriteFile( _fileName, writeMode );
	if ( newFile->IsOpen() ) {
		newFile->Retain();
		return newFile;
	}

	delete( newFile );
	return nullptr;
}

size_t xiWriteFile::Write( const void * const buffer, const size_t sizeToWrite ) {
	if ( IsOpen() == false ) {
		return 0;
	}

	return fwrite( buffer, 1, sizeToWrite, ( FILE * )file );
}

xiWriteFile::xiWriteFile( const char * const _fileName, const writeMode_e writeMode ) : xiFile( _fileName ) {
	OpenFile( writeMode );
}

void xiWriteFile::OpenFile( const writeMode_e writeMode ) {
	if ( filename[0] == 0 ) 	{
		file = 0;
		return;
	}

	if ( writeMode == WRITE_APPEND ) {
		file = fopen( filename, "ab" );
	} else {
		file = fopen( filename, "wb" );
	}

	if ( file ) {
#if defined( __WIN_API__ ) && defined ( __X64__ )
		_fseeki64( ( FILE * )file, 0, SEEK_END );
		fileSize = _ftelli64( ( FILE * )file );
		_fseeki64( ( FILE * )file, 0, SEEK_SET );
#else
		fseek( ( FILE * )file, 0, SEEK_END );
		fileSize = ftell( ( FILE * )file );
		fseek( ( FILE * )file, 0, SEEK_SET );
#endif
	}
}
