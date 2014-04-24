#include "FileSystem.h"

#include <string.h>
#include <stdlib.h>
#include "FileArchive.h"
#include "MemoryFile.h"
#define __UNSAFE__
#include "os.h"
#include "PathUtil.h"

#if defined( __WIN_API__ )
	#include <io.h>
	#include <direct.h>
	#include <tchar.h>
#endif

#include "ZipReader.h"

xiFileSystem *	xiFileSystem::singleton = nullptr;
int32_t			xiFileSystem::references = 0;

xiFileSystem * xiFileSystem::Get() {
	if ( !singleton ) {
		singleton = new xiFileSystem();
	}

	singleton->Retain();
	return singleton;
}

void xiFileSystem::Retain() {
	if ( singleton ) {
		references++;
	}
}

void xiFileSystem::Release() {
	if ( singleton ) {
		references--;

		if ( references == 0 ) {
			delete( singleton );
			singleton = nullptr;
		}
	}
}

void xiFileSystem::ArchiveLoader_PushBack( xiArchiveLoader * const loader ) {
	const size_t biggerArchiveLoadersLen = archiveLoader.archiveLoadersLen + 1;
	xiArchiveLoader ** const biggerArchiveLoaders = ( xiArchiveLoader ** )realloc( archiveLoader.archiveLoaders, sizeof( *biggerArchiveLoaders ) * biggerArchiveLoadersLen );
	
	if ( biggerArchiveLoaders ) {
		biggerArchiveLoaders[archiveLoader.archiveLoadersLen] = loader;

		archiveLoader.archiveLoaders = biggerArchiveLoaders;
		archiveLoader.archiveLoadersLen = biggerArchiveLoadersLen;
	}
}

void xiFileSystem::FileArchives_PushBack( xiFileArchive * const archive ) {
	const size_t biggerArchivesLen = fileArchives.archivesLen + 1;
	xiFileArchive ** const biggerArchives = ( xiFileArchive ** )realloc( fileArchives.archives, sizeof( *biggerArchives ) * biggerArchivesLen );
	
	if ( biggerArchives ) {
		biggerArchives[fileArchives.archivesLen] = archive;

		fileArchives.archives = biggerArchives;
		fileArchives.archivesLen = biggerArchivesLen;
	}
}

void xiFileSystem::FileArchives_EraseAtIndex( const size_t index ) {
	if ( index < fileArchives.archivesLen - 1 ) {
		for ( size_t ii = index; ii < fileArchives.archivesLen - 1; ii++ ) {
			fileArchives.archives[ii] = fileArchives.archives[ii + 1];
		}
	}

	fileArchives.archivesLen -= 1;
}

xiFileSystem::xiFileSystem() {
	memset( &workingDirectory[0], 0, sizeof( char ) * DIRECTORY_LEN );
	memset( &archiveLoader, 0, sizeof( archiveLoader ) );
	memset( &fileArchives, 0, sizeof( fileArchives ) );

	GetWorkingDirectory();

#if defined( ZIP_LOADER )
	ArchiveLoader_PushBack( new xiZipLoader( this ) );
#endif
}

xiFileSystem::~xiFileSystem() {
	if ( fileArchives.archivesLen ) {
		size_t index = fileArchives.archivesLen - 1;

		do {
			delete( fileArchives.archives[index] );
		} while ( index-- );

		free( fileArchives.archives );
	}

	if ( archiveLoader.archiveLoadersLen ) {
		size_t index = archiveLoader.archiveLoadersLen - 1;

		do {
			delete( archiveLoader.archiveLoaders[index] );
		} while ( index-- );

		free( archiveLoader.archiveLoaders );
	}
}

xiReadFile * xiFileSystem::CreateAndOpenFile( const char * const fileName ) {
	xiReadFile * file = nullptr;
		
	if ( fileArchives.archivesLen ) {
		size_t index = fileArchives.archivesLen - 1;

		do {
			file = fileArchives.archives[index]->CreateAndOpenFileName( fileName );
			if ( file ) {
				return file;
			}
		} while ( index-- );
	}

	char absoPath[PATH_LEN];
	GetAbsolutePath( &absoPath[0], fileName );
	return xiReadFile::CreateReadFile( absoPath );
}

xiReadFile * xiFileSystem::CreateMemoryReadFile( void * const memory, const size_t len, const char * const fileName, const deleteType_e deleteType ) {
	if ( !memory ) {
		return nullptr;
	} else {
		return xiReadFile::CreateMemoryReadFile( memory, len, fileName, ( deleteType == DELETE_WHEN_DROPPED ) );
	}
}

xiReadFile * xiFileSystem::CreateLimitReadFile( const char * const fileName, xiReadFile * const alreadyOpenedFile, const size_t pos, const size_t areaSize ) {
	if ( !alreadyOpenedFile ) {
		return nullptr;
	} else {
		return xiReadFile::CreateLimitReadFile( fileName, alreadyOpenedFile, pos, areaSize );
	}
}

xiWriteFile * xiFileSystem::CreateMemoryWriteFile( void * const memory, const size_t len, const char * const fileName, const deleteType_e deleteType ) {
	if ( !memory ) {
		return nullptr;
	} else {
		return ( xiWriteFile * )xiReadFile::CreateMemoryReadFile( memory, len, fileName, ( deleteType == DELETE_WHEN_DROPPED ) );
	}
}

xiWriteFile * xiFileSystem::CreateAndWriteFile( const char * const fileName, const xiWriteFile::writeMode_e writeMode ) {
	return xiWriteFile::CreateWriteFile( fileName, writeMode );
}

void xiFileSystem::AddArchiveLoader( xiArchiveLoader * const loader ) {
	if ( !loader ) {
		return;
	}

	ArchiveLoader_PushBack( loader );
}

size_t xiFileSystem::GetArchiveLoaderCount() const {
	return archiveLoader.archiveLoadersLen;
}

xiArchiveLoader * xiFileSystem::GetArchiveLoader( const size_t index ) const {
	if ( index < archiveLoader.archiveLoadersLen ) {
		return archiveLoader.archiveLoaders[index];
	} else {
		return nullptr;
	}
}

bool xiFileSystem::MoveArchive( const size_t sourceIndex, const int32_t relative ) {
	bool success = false;

	const ptrdiff_t dest = ( ptrdiff_t )sourceIndex + ( ptrdiff_t )relative;
	const char dir = relative < 0 ? -1 : 1;
	const ptrdiff_t sourceEnd = ( ( ptrdiff_t )archiveLoader.archiveLoadersLen ) - 1;
	
	xiFileArchive * temp = nullptr;

	for ( ptrdiff_t ii = ( ptrdiff_t )sourceIndex; ii != dest; ii += dir ) {
		if ( ii < 0 || ii > sourceEnd || ii + dir < 0 || ii + dir > sourceEnd ) {
			continue;
		}

		temp = fileArchives.archives[ii + dir];
		fileArchives.archives[ii + dir] = fileArchives.archives[ii];
		fileArchives.archives[ii] = temp;

		success = true;
	}

	return success;
}

bool xiFileSystem::AddArchiveFile( const char * const fileName, const xiFileList::ignoreParam_e ignoreMask, xiFileArchive ** const outArchive ) {
	if ( !archiveLoader.archiveLoadersLen ) {
		return false;
	}
	
	xiFileArchive * archive = nullptr;
	bool ret = false;
	
	// Try to load archive based on file name
	size_t ii = archiveLoader.archiveLoadersLen - 1;
	do {
		if ( archiveLoader.archiveLoaders[ii]->IsLoadableFileName( fileName ) ) {
			archive = archiveLoader.archiveLoaders[ii]->CreateArchiveFromFileName( fileName, ignoreMask );
			if ( archive ) {
				break;
			}
		}
	} while ( ii-- );

	// Try to load archive based on content
	if ( !archive )	{
		xiReadFile * const file = CreateAndOpenFile( fileName );
		if ( file ) {
			ii = archiveLoader.archiveLoadersLen - 1;
			do {
				file->Seek( 0 );

				if ( archiveLoader.archiveLoaders[ii]->IsLoadableReadFile( file ) ) {
					file->Seek( 0 );
					archive = archiveLoader.archiveLoaders[ii]->CreateArchiveFromReadFile( file, ignoreMask );
					if ( archive ) {
						break;
					}
				}
			} while ( ii-- );

			file->Release();
		}
	}

	if ( archive ) {
		FileArchives_PushBack( archive );

		if ( outArchive ) {
			*outArchive = archive;
		}

		ret = true;
	}

	return ret;
}

bool xiFileSystem::AddArchiveReadFile( xiReadFile * const readFile, const xiFileList::ignoreParam_e ignoreMask, xiFileArchive ** const outArchive ) {
	if ( !readFile || !archiveLoader.archiveLoadersLen ) {
		return false;
	}

	xiFileArchive * archive = nullptr;

	size_t ii = archiveLoader.archiveLoadersLen - 1;
	do {
		if ( archiveLoader.archiveLoaders[ii]->IsLoadableFileName( readFile->GetFileName() ) ) {
			archive = archiveLoader.archiveLoaders[ii]->CreateArchiveFromReadFile( readFile, ignoreMask );
			if ( archive ) {
				break;
			}
		}
	} while ( ii-- );
	
	// try to load archive based on content
	if ( !archive ) {
		ii = archiveLoader.archiveLoadersLen - 1;
		do {
			readFile->Seek( 0 );

			if ( archiveLoader.archiveLoaders[ii]->IsLoadableReadFile( readFile ) ) {
				readFile->Seek( 0 );
				archive = archiveLoader.archiveLoaders[ii]->CreateArchiveFromReadFile( readFile, ignoreMask );
				if ( archive ) {
					break;
				}
			}
		} while ( ii-- );
	}

	if ( archive ) {
		FileArchives_PushBack( archive );

		if ( outArchive ) {
			*outArchive = archive;
		}

		return true;
	}

	return false;
}

bool xiFileSystem::AddArchive( xiFileArchive * const archive ) {
	if ( fileArchives.archivesLen ) {
		size_t ii = fileArchives.archivesLen - 1;

		do {
			if ( archive == fileArchives.archives[ii] ) {
				return false;
			}
		} while ( ii-- );
	}

	FileArchives_PushBack( archive );
	return true;
}

bool xiFileSystem::RemoveArchiveAtIndex( const size_t index ) {
	bool ret = false;
	if ( index < fileArchives.archivesLen ) {
		delete( fileArchives.archives[index] );

		FileArchives_EraseAtIndex( index );

		ret = true;
	}
	
	return ret;
}

bool xiFileSystem::RemoveArchiveFile( const char * const fileName ) {
	if ( !fileArchives.archivesLen ) {
		return false;
	}

	char absPath[PATH_LEN];
	GetAbsolutePath( &absPath[0], fileName );

	size_t ii = fileArchives.archivesLen - 1;

	do {
		if ( strcmp( absPath, fileArchives.archives[ii]->GetFileList()->GetPath() ) == 0 ) {
			return RemoveArchiveAtIndex( ii );
		}
	} while ( ii-- );

	return false;
}

bool xiFileSystem::RemoveArchive( const xiFileArchive * const archive ) {
	if ( !fileArchives.archivesLen ) {
		return false;
	}

	size_t ii = fileArchives.archivesLen - 1;

	do {
		if ( archive == fileArchives.archives[ii] ) {
			return RemoveArchiveAtIndex( ii );
		}
	} while ( ii-- );

	return false;
}

size_t xiFileSystem::GetArchiveCount() const {
	return fileArchives.archivesLen;
}

xiFileArchive * xiFileSystem::GetArchive( const size_t index ) const {
	return ( index < fileArchives.archivesLen ? fileArchives.archives[index] : nullptr );
}

const char * xiFileSystem::GetWorkingDirectory() {
	memset( &workingDirectory[0], 0, sizeof( char ) * DIRECTORY_LEN );

#if defined( __WIN_API__ )
	_getcwd( workingDirectory, DIRECTORY_LEN );
	PathUtil::UnixSlashes( &workingDirectory[0] );
#elif defined( __POSIX__ )
	#error "Deal with posix working directory"
#endif

	return workingDirectory;
}

bool xiFileSystem::ChangeWorkingDirectoryTo( const char * const newDirectory ) {
	bool success = false;

#if defined( __WIN_API__ )
	success = ( _chdir( newDirectory ) == 0 );
#elif defined( __POSIX__ )
	#error "Deal with posix fs"
#endif

	if ( success ) {
		PathUtil::SafeStrCopy( &workingDirectory[0], newDirectory, DIRECTORY_LEN );
	}

	return success;
}

void xiFileSystem::GetAbsolutePath( char * const dest, const char * const source ) const {
#if defined( __WIN_API__ )
	char * p = 0;
	char fpath[PATH_LEN];
	p = _fullpath( fpath, source, PATH_LEN );

	PathUtil::SafeStrCopy( dest, p, PATH_LEN );
	PathUtil::UnixSlashes( dest );
#elif defined( __POSIX__ )
	#error "Deal with posix fs"
#endif
}

void xiFileSystem::GetFileDirectory( char * const dest, const char * const fileName ) const {
	ptrdiff_t lastSlash = strlen( fileName );
	while ( lastSlash-- && fileName[lastSlash] != '/' ) {};
	
	ptrdiff_t lastBackSlash = strlen( fileName );
	while ( lastBackSlash-- && fileName[lastBackSlash] != '\\' ) {};
	
	lastSlash = ( lastSlash > lastBackSlash ? lastSlash : lastBackSlash );

	if ( lastSlash < ( ptrdiff_t )strlen( fileName ) ) {
		ptrdiff_t index = 0;

		do {
			dest[index] = fileName[index];
		} while ( index++ < lastSlash );
	} else {
		dest[0] = '.';
		dest[1] = 0;
	}
}

void xiFileSystem::GetFileBasename( char * const dest, const char * const fileName, const extensionHandler_e extHandler ) const {
	ptrdiff_t lastSlash = strlen( fileName );
	while ( lastSlash-- && fileName[lastSlash] != '/' ) {};
	
	ptrdiff_t lastBackSlash = strlen( fileName );
	while ( lastBackSlash-- && fileName[lastBackSlash] != '\\' ) {};
	
	lastSlash = ( lastSlash > lastBackSlash ? lastSlash : lastBackSlash );

	// get number of chars after last dot
	ptrdiff_t end = 0;
	if ( extHandler != EXTENSION_KEEP ) {
		// take care to search only after last slash to check only for
		// dots in the filename
		end = strlen( fileName );
		while ( end-- && fileName[end] != '.' );

		if ( end == -1 || end < lastSlash ) {
			end = 0;
		} else {
			end = strlen( fileName ) - end;
		}
	}

	if ( ( size_t )lastSlash < strlen( fileName ) ) {
		strncat( dest, fileName + lastSlash + 1, strlen( fileName ) - lastSlash - 1 - end );
	} else if (end != 0) {
		strncat( dest, fileName, strlen( fileName ) - end );
	} else {
		PathUtil::SafeStrCopy( dest, fileName, FILE_NAME_LEN );
	}
}

void xiFileSystem::GetFlatFilename( char * const dest, const char * const directory, const char * const root ) const {
	char dirTemp[PATH_LEN];
	PathUtil::SafeStrCopy( &dirTemp[0], directory, PATH_LEN - 1 );
	PathUtil::UnixSlashes( &dirTemp[0] );

	const size_t dirTempLen = strlen( dirTemp );
	if ( dirTemp[dirTempLen - 1] != '/' ) {
		const size_t dLen = ( dirTempLen < ( PATH_LEN - 2 ) ? dirTempLen : PATH_LEN - 2 );
		dirTemp[dLen] = '/';
		dirTemp[dLen + 1] = 0;
	}

	char dir[PATH_LEN];
	char subdir[PATH_LEN];
		
	ptrdiff_t lastpos = 0;
	ptrdiff_t pos = 0;
	bool lastWasRealDir = false;

	while ( ( pos = PathUtil::FindNext( dirTemp, '/', lastpos ) ) >= 0 ) {
		strncat( subdir, dirTemp + lastpos, pos - lastpos + 1 );

		if ( strcmp( subdir, "../" ) == 0 ) {
			if ( lastWasRealDir ) {
				PathUtil::DeletePathFromPath( dir, dir, 2 );
				lastWasRealDir = ( dir[0] != 0 );
			} else {
				PathUtil::SafeStrCopy( &dir[strlen( dir )], subdir, PATH_LEN - strlen( dir ) );

				lastWasRealDir = false;
			}
		} else if ( strcmp( subdir, "/" ) == 0 ) {
			PathUtil::SafeStrCopy( &dir[0], root, PATH_LEN );
		} else if ( strcmp( subdir, "./" ) != 0 ) {
			PathUtil::SafeStrCopy( &dir[strlen( dir )], subdir, PATH_LEN - strlen( dir ) );
			
			lastWasRealDir = true;
		}

		lastpos = pos + 1;
	}

	PathUtil::SafeStrCopy( dest, &dir[0], FILE_NAME_LEN );
}

xiFileList * xiFileSystem::CreateFileList() {
	xiFileList * r = nullptr;
	char path[PATH_LEN];
	PathUtil::SafeStrCopy( &path[0], GetWorkingDirectory(), PATH_LEN - 1 );
	PathUtil::UnixSlashes( path );

	const size_t pathLen = strlen( path );
	if ( path[pathLen] != '/') {
		const size_t pLen = ( pathLen < ( PATH_LEN - 2 ) ? pathLen : PATH_LEN - 2 );
		path[pLen] = '/';
		path[pLen + 1] = 0;
	}

#if defined( __WIN_API__ )

	r = new xiFileList( path, xiFileList::IGNORE_CASE );

	intptr_t hFile;

	struct _tfinddata_t c_file;
	if ( ( hFile = _tfindfirst( _T( "*" ), &c_file ) ) != -1L ) {
		do {
			char rPath[PATH_LEN];
			PathUtil::SafeStrCopy( &rPath[0], &path[0], PATH_LEN );

			const size_t rLen = strlen( rPath );
			const size_t wcFileNameLen = wcslen( c_file.name );

			size_t ii;
			for ( ii = 0; ii < wcFileNameLen; ii++ ) {
				if ( rLen + ii >= PATH_LEN ) {
					break;
				}
				rPath[rLen + ii] = ( char )c_file.name[ii];
			}
			rPath[rLen + ii] = 0;

			r->AddItem( &rPath[0], 0, ( size_t )c_file.size, ( _A_SUBDIR & c_file.attrib ? xiFileList::ITEM_DIRECTORY : xiFileList::ITEM_FILE ), 0);
		} while( _tfindnext( hFile, &c_file ) == 0 );

		_findclose( hFile );
	}

#elif defined( __POSIX__)
	#error "Deal with it"
#endif

	r->Retain();
	r->Sort();
	
	return r;
}

xiFileList * xiFileSystem::CreateEmptyFileList( const char * const path, const xiFileList::ignoreParam_e ignoreMask ) {
	return new xiFileList( path, ignoreMask );
}

bool xiFileSystem::DoesFileExist( const char * const fileName ) const {
	if ( fileArchives.archivesLen ) {
		size_t ii = fileArchives.archivesLen - 1;
		do {
			if ( fileArchives.archives[ii]->GetFileList()->FindFile( fileName, xiFileList::ITEM_FILE ) != -1 ) {
				return true;
			}
		} while ( ii-- );
	}

#if defined( __WIN_API__ )
	return ( _access( fileName, 0 ) != -1 );
#endif
}