#ifndef __LIMIT_READ_FILE_H__
#define __LIMIT_READ_FILE_H__

#include "ReadFile.h"

class xiLimitReadFile : public xiReadFile {
friend xiReadFile;
public:
	virtual					~xiLimitReadFile();
	virtual size_t			Read( void * const buffer, const size_t sizeToRead );
	virtual bool			Seek( const ptrdiff_t finalPos, const seekType_e seekType = SEEK_ABSOLUTE );
	virtual size_t			GetSize() const;
	virtual size_t			GetPos() const;
	virtual const char *	GetFileName() const;
protected:
	xiLimitReadFile( xiReadFile * const alreadyOpenedFile, const size_t _pos, const size_t areaSize, const char * const name );
private:
	char			filename[FILE_NAME_LEN]; 
	size_t			areaStart;
	size_t			areaEnd;
	size_t			pos;
	xiReadFile *	rFile;
};

#endif