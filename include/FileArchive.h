#ifndef __FILE_ARCHIVE_H__
#define __FILE_ARCHIVE_H__

#include "FileList.h"

class xiReadFile;

class xiFileArchive {
public:
	virtual						~xiFileArchive() {};
	virtual xiReadFile *		CreateAndOpenFileName( const char * const fileName ) = 0;
	virtual xiReadFile *		CreateAndOpenFileIndex( const size_t fileIndex ) = 0;
	virtual const xiFileList *	GetFileList() const = 0;
};

class xiArchiveLoader {
public:
	virtual					~xiArchiveLoader() {};
	virtual bool			IsLoadableFileName( const char * const fileName ) const = 0;
	virtual bool			IsLoadableReadFile( xiReadFile * const readFile ) const = 0;
	virtual xiFileArchive *	CreateArchiveFromFileName( const char * const fileName, const xiFileList::ignoreParam_e ignoreMask ) const = 0;
	virtual xiFileArchive *	CreateArchiveFromReadFile( xiReadFile * const readFile, const xiFileList::ignoreParam_e ignoreMask ) const = 0;
};

#endif