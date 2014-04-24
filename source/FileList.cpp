#include "FileList.h"

#include <string.h>
#include <stdlib.h>
#define __UNSAFE__
#include "os.h"
#include "PathUtil.h"

#define MIN( X, Y )	( X < Y ? X : Y )

typedef int ( * compareFun_t )( const void *, const void * );

bool xiFileList::FileListEntry_IsEqual( const fileListEntry_t * const left, const fileListEntry_t * const right ) {
	if ( left->itemType != right->itemType ) {
		return false;
	}

	size_t leftLen = strlen( left->fullName );
	const size_t rightLen = strlen( right->fullName );

	if ( leftLen != rightLen ) {
		return false;
	}

	do {
		if ( left->fullName[leftLen] != right->fullName[leftLen] ) {
			return false;
		}
	} while ( leftLen-- );

	return true;
}

int xiFileList::FileListEntry_Compare( const fileListEntry_t * const left, const fileListEntry_t * const right ) {
	if ( left->itemType != right->itemType ) {
		return ( left->itemType == ITEM_DIRECTORY ? -1 : 1 );
	}

	const size_t leftLen = strlen( left->fullName );
	const size_t rightLen = strlen( right->fullName );

	size_t index = MIN( leftLen, rightLen );
	
	for ( size_t ii = 0; ii < index; ii++ ) {
		if ( left->fullName[ii] < right->fullName[ii] ) {
			return -1; // An ASCII char is less than
		} else if ( left->fullName[ii] > right->fullName[ii] ) {
			return 1;
		}
	}

	if ( leftLen < rightLen ) {
		return -1;
	} else if ( leftLen > rightLen ) {
		return 1;
	}

	return 0;
}

void xiFileList::PushBackFile( fileListEntry_t * const entry ) {
	const size_t biggerFileListLen = fileListLen + 1;
	
	fileListEntry_t * const biggerFileList = ( fileListEntry_t * )realloc( fileList, sizeof( *biggerFileList ) * biggerFileListLen );
	if ( biggerFileList ) {
		memcpy( &biggerFileList[fileListLen], entry, sizeof( *entry ) );

		fileList = biggerFileList;
		fileListLen = biggerFileListLen;
	}
}

xiFileList::xiFileList( const char * const _path, const int32_t _ignoreMask ) {
	PathUtil::SafeStrCopy( path, _path, PATH_LEN );

	ignoreMask = _ignoreMask;
	PathUtil::UnixSlashes( &path[0] );

	fileList = 0;
	fileListLen = 0;
	references = 0;
}

xiFileList::~xiFileList() {
	free( fileList );
}

size_t xiFileList::AddItem( const char * const fullPath, const size_t offset, const size_t size, const itemType_e itemType, const size_t id ) {
	fileListEntry_t entry;
	entry.ID = ( id ? id : fileListLen );
	entry.offset = offset;
	entry.size = size;
	PathUtil::SafeStrCopy( &entry.fullName[0], fullPath, PATH_LEN );
	PathUtil::UnixSlashes( &entry.fullName[0] );
	entry.itemType = itemType;
	
	// remove trailing slash
	if (entry.fullName[strlen( entry.fullName )] == '/') {
		entry.itemType = ITEM_DIRECTORY;
		entry.fullName[strlen( entry.fullName )] = 0;
	}

	if ( ignoreMask & IGNORE_CASE ) {
		PathUtil::ToLower( &entry.fullName[0] );
	}

	PathUtil::DeletePath( &entry.name[0], &entry.fullName[0] );

	if ( ignoreMask & IGNORE_PATHS ) {
		PathUtil::SafeStrCopy( &entry.fullName[0], &entry.name[0], PATH_LEN );
	}

	PushBackFile( &entry );

	return fileListLen - 1;
}

void xiFileList::Sort() {
	qsort( fileList, fileListLen, sizeof( *fileList ), ( compareFun_t )&FileListEntry_Compare );
}

size_t xiFileList::GetFileCount() const {
	return fileListLen;
}

const char * xiFileList::GetFileName( const size_t index ) const {
	if ( index >= fileListLen ) {
		return "";
	}

	return fileList[index].name;
}

const char * xiFileList::GetFullFileName( const size_t index ) const {
	if ( index >= fileListLen ) {
		return "";
	}

	return fileList[index].fullName;
}

size_t xiFileList::GetID( const size_t index ) const {
	return ( index < fileListLen ? fileList[index].ID : 0 );
}

bool xiFileList::IsDirectory( const size_t index ) const {
	bool ret = false;
	if ( index < fileListLen ) {
		ret = fileList[index].itemType == ITEM_DIRECTORY;
	}

	return ret;
}

size_t xiFileList::GetFileSize( const size_t index ) const {
	return ( index < fileListLen ? fileList[index].size : 0 );
}

size_t xiFileList::GetFileOffset( const size_t index ) const {
	return ( index < fileListLen ? fileList[index].offset : 0 );
}

ptrdiff_t xiFileList::FindFile( const char * const filename, const itemType_e itemType ) const {
	if ( fileListLen == 0 ) {
		return -1;
	}

	fileListEntry_t entry;
	// We only need FullName to be set for the search
	PathUtil::SafeStrCopy( &entry.fullName[0], filename, PATH_LEN );
	entry.itemType = itemType;

	// Exchange
	PathUtil::UnixSlashes( &entry.fullName[0] );

	// Remove trailing slash
	if ( entry.fullName[strlen( entry.fullName ) - 1] == '/' )	{
		entry.itemType = ITEM_DIRECTORY;
		entry.fullName[strlen( entry.fullName ) - 1] = 0;
	}

	if ( ignoreMask & IGNORE_CASE ) {
		PathUtil::ToLower( &entry.fullName[0] );
	}

	if ( ignoreMask & IGNORE_PATHS ) {
		PathUtil::DeletePath( &entry.name[0], &entry.fullName[0] );
		PathUtil::DeletePath( &entry.fullName[0], &entry.name[0] );
	}

	size_t index = fileListLen - 1;
	do {
		if ( FileListEntry_IsEqual( &fileList[index], &entry ) ) {
			return index;
		}
	} while ( index-- );
	
	return -1;
}

const char * xiFileList::GetPath() const {
	return path;
}

void xiFileList::Retain() {
	references++;
}

void xiFileList::Release() {
	references--;

	if ( !references ) {
		delete( this );
	}
}