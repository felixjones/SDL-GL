#ifndef __WRITE_FILE_H__
#define __WRITE_FILE_H__

#include "File.h"

class xiWriteFile : public xiFile {
public:
	enum writeMode_e {
		WRITE_OVER,
		WRITE_APPEND
	};
	
	static xiWriteFile *	CreateWriteFile( const char * const _fileName, const writeMode_e writeMode = WRITE_OVER );
	virtual size_t			Write( const void * const buffer, const size_t sizeToWrite );
protected:
	xiWriteFile( const char * const _fileName, const writeMode_e writeMode );
private:
	void	OpenFile( const writeMode_e writeMode );
};

#endif