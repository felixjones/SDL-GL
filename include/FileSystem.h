#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <stdint.h>
#include "ReadFile.h"
#include "WriteFile.h"
#include "FileList.h"

#define DIRECTORY_LEN	( 240 )

class xiFileArchive;
class xiArchiveLoader;

class xiFileSystem {
public:
	enum deleteType_e {
		DELETE_WHEN_DROPPED,
		DELETE_NEVER
	};

	enum extensionHandler_e {
		EXTENSION_REMOVE,
		EXTENSION_KEEP
	};

	static xiFileSystem *	Get();
	void					Retain();
	void					Release();
	xiReadFile *			CreateAndOpenFile( const char * const fileName );
	xiReadFile *			CreateMemoryReadFile( void * const memory, const size_t len, const char * const fileName, const deleteType_e deleteType = DELETE_NEVER );
	xiReadFile *			CreateLimitReadFile( const char * const fileName, xiReadFile * const alreadyOpenedFile, const size_t pos, const size_t areaSize );
	xiWriteFile *			CreateMemoryWriteFile( void * const memory, const size_t len, const char * const fileName, const deleteType_e deleteType = DELETE_NEVER );
	xiWriteFile *			CreateAndWriteFile( const char * const fileName, const xiWriteFile::writeMode_e writeMode = xiWriteFile::WRITE_OVER );
	bool					AddArchiveFile( const char * const fileName, const xiFileList::ignoreParam_e ignoreMask = xiFileList::IGNORE_NONE, xiFileArchive ** const outArchive = nullptr );
	bool					AddArchiveReadFile( xiReadFile * const readFile, const xiFileList::ignoreParam_e ignoreMask = xiFileList::IGNORE_NONE, xiFileArchive ** const outArchive = nullptr );
	bool					AddArchive( xiFileArchive * const archive );
	bool					MoveArchive( const size_t sourceIndex, const int32_t relative );
	void					AddArchiveLoader( xiArchiveLoader * const loader );
	size_t					GetArchiveLoaderCount() const;
	xiArchiveLoader *		GetArchiveLoader( const size_t index ) const;
	size_t					GetArchiveCount() const;
	xiFileArchive *			GetArchive( const size_t index ) const;
	bool					RemoveArchiveAtIndex( const size_t index );
	bool					RemoveArchiveFile( const char * const fileName );
	bool					RemoveArchive( const xiFileArchive * const archive );
	const char *			GetWorkingDirectory();
	bool					ChangeWorkingDirectoryTo( const char * const newDirectory );
	void					GetAbsolutePath( char * const dest, const char * const source ) const;
	void					GetFileDirectory( char * const dest, const char * const fileName ) const;
	void					GetFileBasename( char * const dest, const char * const fileName, const extensionHandler_e extHandler = EXTENSION_KEEP ) const;
	void					GetFlatFilename( char * const dest, const char * const directory, const char * const root = "/" ) const;
	xiFileList *			CreateFileList();
	xiFileList *			CreateEmptyFileList( const char * const path, const xiFileList::ignoreParam_e ignoreMask );
	bool					DoesFileExist( const char * const fileName ) const;
protected:
			xiFileSystem();
	virtual	~xiFileSystem();
private:
	void	ArchiveLoader_PushBack( xiArchiveLoader * const loader );
	void	FileArchives_PushBack( xiFileArchive * const archive );
	void	FileArchives_EraseAtIndex( const size_t index );

	char	workingDirectory[DIRECTORY_LEN];

	struct archiveLoader_s {
		xiArchiveLoader **	archiveLoaders;
		size_t				archiveLoadersLen;
	} archiveLoader;

	struct archive_s {
		xiFileArchive **	archives;
		size_t				archivesLen;
	} fileArchives;

	static xiFileSystem *	singleton;
	static int32_t			references;
};

#endif