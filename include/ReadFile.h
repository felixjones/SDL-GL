#ifndef __READ_FILE_H__
#define __READ_FILE_H__

#include "File.h"

// const io::path& fileName, IReadFile* alreadyOpenedFile, long pos, long areaSize

class xiReadFile : public xiFile {
public:
	static xiReadFile *	CreateReadFile( const char * const _fileName );
	static xiReadFile *	CreateLimitReadFile( const char * const _fileName, xiReadFile * const alreadyOpenedFile, const size_t pos, const size_t areaSize );
	static xiReadFile *	CreateMemoryReadFile( void * const memory, const size_t size, const char * const fileName, const bool deleteMemoryWhenDropped );
	virtual size_t		Read( void * const buffer, const size_t sizeToRead );
	virtual size_t		GetSize() const;
protected:
	xiReadFile( const char * const fileName );
private:
	void	OpenFile();
};

#endif