#ifndef __ZIP_READER_H__
#define __ZIP_READER_H__

#include "FileArchive.h"
#include "FileList.h"

class xiFileSystem;
typedef struct zipFileEntry_s zipFileEntry_t;

class xiZipLoader : public xiArchiveLoader {
public:
							xiZipLoader( xiFileSystem * const fs );
	virtual					~xiZipLoader();
	virtual bool			IsLoadableFileName( const char * const fileName ) const;
	virtual bool			IsLoadableReadFile( xiReadFile * const readFile ) const;
	virtual xiFileArchive *	CreateArchiveFromFileName( const char * const fileName, const xiFileList::ignoreParam_e ignoreMask ) const;
	virtual xiFileArchive *	CreateArchiveFromReadFile( xiReadFile * const readFile, const xiFileList::ignoreParam_e ignoreMask ) const;
private:
	xiFileSystem *	fileSystem;
};

class xiZipReader : public virtual xiFileArchive, virtual xiFileList {
public:
								xiZipReader( xiReadFile * const file, const xiFileList::ignoreParam_e ignoreMask );
	virtual						~xiZipReader();
	virtual xiReadFile *		CreateAndOpenFileName( const char * const fileName );
	virtual xiReadFile *		CreateAndOpenFileIndex( const size_t fileIndex );
	virtual const xiFileList *	GetFileList() const;
protected:
	bool	ScanZipHeader( const bool ignoreGPBits );
	bool	ScanCentralDirectoryHeader();
	void	FileInfo_Clear();
	void	FileInfo_PushBack( zipFileEntry_t * const entry );
	void	Files_Clear();

	struct fileInfo_s {
		zipFileEntry_t *	entries;
		size_t				entriesLen;
	} fileInfo;

	xiReadFile *	readFile;
};

#define ZIP_LOADER

#endif