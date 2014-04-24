#ifndef __FILE_LIST_H__
#define __FILE_LIST_H__

#include <stdint.h>
#include <stddef.h>

#define PATH_LEN	( 240 )
#define NAME_LEN	( 80 )

class xiFileList {
public:
	enum itemType_e {
		ITEM_FILE,
		ITEM_DIRECTORY
	};

	enum ignoreParam_e {
		IGNORE_NONE				=	0x0,
		IGNORE_CASE				=	0x1,
		IGNORE_PATHS			=	0x2,
		IGNORE_CASE_AND_PATHS	=	0x3
	};

							xiFileList( const char * const _path, const int32_t _ignoreMask );
	virtual					~xiFileList();
	virtual size_t			AddItem( const char * const fullPath, const size_t offset, const size_t size, const itemType_e itemType, const size_t id = 0 );
	virtual void			Sort();
	virtual size_t			GetFileCount() const;
	virtual const char *	GetFileName( const size_t index ) const;
	virtual const char *	GetFullFileName( const size_t index ) const;
	virtual size_t			GetID( const size_t index ) const;
	virtual bool			IsDirectory( const size_t index ) const;
	virtual size_t			GetFileSize( const size_t index ) const;
	virtual size_t			GetFileOffset( const size_t index ) const;
	virtual ptrdiff_t		FindFile( const char * const filename, const itemType_e itemType ) const;
	virtual const char *	GetPath() const;
	void					Retain();
	void					Release();
protected:
	typedef struct fileListEntry_s {
		char		name[NAME_LEN];
		char		fullName[PATH_LEN];
		size_t		size;
		size_t		ID;
		size_t		offset;
		itemType_e	itemType;
	} fileListEntry_t;

	static bool	FileListEntry_IsEqual( const fileListEntry_t * const left, const fileListEntry_t * const right );
	static int	FileListEntry_Compare( const fileListEntry_t * const left, const fileListEntry_t * const right );

	int32_t				ignoreMask;
	char				path[PATH_LEN];
	fileListEntry_t *	fileList;
	size_t				fileListLen;
	int32_t				references;
private:
	void	PushBackFile( fileListEntry_t * const entry );
};

#endif