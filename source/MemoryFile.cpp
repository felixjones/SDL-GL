#include "MemoryFile.h"

#include <string.h>

xiMemoryFile::~xiMemoryFile() {
	if ( deleteMemoryWhenDropped ) {
		delete[]( ( char * )buffer );
	}
}

size_t xiMemoryFile::Read( void * const _buffer, const size_t sizeToRead ) {
	ptrdiff_t amount = sizeToRead;
	if ( pos + amount > len ) {
		amount -= pos + amount - len;
	}

	if ( amount <= 0 ) {
		return 0;
	}

	const char * const pointer = ( char * )buffer;
	memcpy( _buffer, pointer + pos, amount );

	pos += amount;

	return amount;
}

size_t xiMemoryFile::Write( const void * const _buffer, const size_t sizeToWrite ) {
	ptrdiff_t amount = sizeToWrite;
	if ( pos + amount > len ) {
		amount -= pos + amount - len;
	}

	if ( amount <= 0 ) {
		return 0;
	}
	
	char * const pointer = ( char * )buffer;
	memcpy( pointer + pos, _buffer, amount);

	pos += amount;

	return amount;
}

bool xiMemoryFile::Seek( const ptrdiff_t finalPos, const seekType_e seekType ) {
	if ( seekType == SEEK_RELATIVE ) {
		if ( pos + finalPos > len ) {
			return false;
		}

		pos += finalPos;
	} else {
		if ( finalPos > ( ptrdiff_t )len ) {
			return false;
		}

		pos = finalPos;
	}

	return true;
}

size_t xiMemoryFile::GetSize() const {
	return len;
}

size_t xiMemoryFile::GetPos() const {
	return pos;
}

const char * xiMemoryFile::GetFileName() const {
	return filename;
}

xiMemoryFile::xiMemoryFile( void * const memory, const size_t _len, const char * const _fileName, const bool _deleteMemoryWhenDropped ) : xiReadFile( "" ), xiWriteFile( "", WRITE_OVER ) {
	memset( &filename[0], 0, sizeof( char ) * FILE_NAME_LEN );
	const size_t fileNameLen = strlen( _fileName );
	const size_t copyLen = ( fileNameLen < FILE_NAME_LEN ? fileNameLen : FILE_NAME_LEN );
	memcpy( &filename[0], &_fileName[0], sizeof( char ) * copyLen );

	buffer = memory;
	len = _len;
	pos = 0;
	deleteMemoryWhenDropped = _deleteMemoryWhenDropped;
}

xiReadFile * xiReadFile::CreateMemoryReadFile( void * const memory, const size_t size, const char * const fileName, const bool deleteMemoryWhenDropped ) {
	xiReadFile * const memoryFile = new xiMemoryFile( memory, size, fileName, deleteMemoryWhenDropped );
	memoryFile->Retain();
	return memoryFile;
}