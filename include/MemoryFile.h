#ifndef __MEMORY_FILE_H__
#define __MEMORY_FILE_H__

#include "ReadFile.h"
#include "WriteFile.h"

class xiMemoryFile : public xiReadFile, public xiWriteFile {
friend xiReadFile;
public:
	virtual					~xiMemoryFile();
	virtual size_t			Read( void * const _buffer, const size_t sizeToRead );
	virtual size_t			Write( const void * const _buffer, const size_t sizeToWrite );
	virtual bool			Seek( const ptrdiff_t finalPos, const seekType_e seekType = SEEK_ABSOLUTE );
	virtual size_t			GetSize() const;
	virtual size_t			GetPos() const;
	virtual const char *	GetFileName() const;
protected:
	xiMemoryFile( void * const memory, const size_t _len, const char * const _fileName, const bool _deleteMemoryWhenDropped );
private:
	void *	buffer;
	size_t	len;
	size_t	pos;
	char	filename[FILE_NAME_LEN];
	bool	deleteMemoryWhenDropped;
};

#endif