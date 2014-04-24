#ifndef __FILE_H__
#define __FILE_H__

#include <stdint.h>

#define FILE_NAME_LEN	( 80 )

class xiFile {
public:
	enum seekType_e {
		SEEK_ABSOLUTE,
		SEEK_RELATIVE
	};
	
	void					Retain();
	void					Release();
	virtual bool			Seek( const ptrdiff_t finalPos, const seekType_e seekType = SEEK_ABSOLUTE );
	virtual bool			IsOpen() const;
	virtual size_t			GetPos() const;
	virtual const char *	GetFileName() const;
protected:
			xiFile( const char * const _fileName );
	virtual	~xiFile();

	void *	file;
	char	filename[FILE_NAME_LEN];
	size_t	fileSize;
	int32_t	references;
};

#endif