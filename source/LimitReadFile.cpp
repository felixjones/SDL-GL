#include "LimitReadFile.h"

#include <string.h>

#define MIN( X, Y )			( X < Y ? X : Y )
#define MAX( X, Y )			( X > Y ? X : Y )
#define CLAMP( X, Y, Z )	MIN( MAX( X, Y ), Z )

size_t xiLimitReadFile::Read( void * const buffer, const size_t sizeToRead ) {
	if ( !rFile ) {
		return 0;
	}

	size_t read = areaStart + pos;
	const ptrdiff_t toRead = MIN( areaEnd, read + sizeToRead ) - MAX( areaStart, read );

	if ( toRead < 0 ) {
		return 0;
	}

	rFile->Seek( read );
	read = rFile->Read( buffer, toRead );

	pos += read;

	return read;
}

bool xiLimitReadFile::Seek( const ptrdiff_t finalPos, const seekType_e seekType ) {
	pos = CLAMP( finalPos + ( seekType == SEEK_RELATIVE ? pos : 0 ), 0, areaEnd - areaStart );

	return true;
}

size_t xiLimitReadFile::GetSize() const {
	return areaEnd - areaStart;
}

size_t xiLimitReadFile::GetPos() const {
	return pos;
}

const char * xiLimitReadFile::GetFileName() const {
	return filename;
}

xiLimitReadFile::xiLimitReadFile( xiReadFile * const alreadyOpenedFile, const size_t _pos, const size_t areaSize, const char * const name ) : xiReadFile( "" ) {
	memset( &filename[0], 0, sizeof( char ) * FILE_NAME_LEN );
	const size_t fileNameLen = strlen( name );
	const size_t copyLen = ( fileNameLen < FILE_NAME_LEN ? fileNameLen : FILE_NAME_LEN );
	memcpy( &filename[0], &name[0], sizeof( char ) * copyLen );
	
	areaStart = 0;
	areaEnd = 0;
	pos = 0;
	rFile = alreadyOpenedFile;

	if ( rFile ) {
		rFile->Retain();

		areaStart = _pos;
		areaEnd = areaStart + areaSize;
	}
}

xiLimitReadFile::~xiLimitReadFile() {
	if ( rFile ) {
		rFile->Release();
	}
}

xiReadFile * xiReadFile::CreateLimitReadFile( const char * const _fileName, xiReadFile * const alreadyOpenedFile, const size_t pos, const size_t areaSize ) {
	xiReadFile * const limitFile = new xiLimitReadFile( alreadyOpenedFile, pos, areaSize, _fileName );
	limitFile->Retain();

	return limitFile;
}
