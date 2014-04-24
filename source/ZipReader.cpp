#include "ZipReader.h"

#include <string.h>
#include <stdlib.h>
#include "FileSystem.h"
#include "PathUtil.h"
#include "os.h"
#include "zlib.h"

#if defined( __POSIX__ )
	#include <machine/endian.h>
#endif

#define ZIP_INFO_IN_DATA_DESCRIPTOR	( ( int16_t )0x0008 )

#include "bytePack.h"

typedef struct zipFileDataDescriptor_s {
	uint32_t	crc32;
	uint32_t	compressedSize;
	uint32_t	uncompressedSize;
} PACK_STRUCT zipFileDataDescriptor_t;

typedef struct zipFileHeader_s {
	uint32_t				sig; // 'PK0304' little endian (0x04034b50)
	int16_t					versionToExtract;
	int16_t					generalBitFlag;
	int16_t					compressionMethod;
	int16_t					lastModFileTime;
	int16_t					lastModFileDate;
	zipFileDataDescriptor_t	dataDescriptor;
	int16_t					fileNameLength;
	int16_t					extraFieldLength;
	// fileName ( variable length )
	// extraField ( variable length )
} PACK_STRUCT zipFileHeader_t;

typedef struct zipFileCentralDirFileHeader_s {
	uint32_t	sig; // 'PK0102' (0x02014b50)
	uint16_t	versionMadeBy;
	uint16_t	versionToExtract;
	uint16_t	generalBitFlag;
	uint16_t	compressionMethod;
	uint16_t	lastModFileTime;
	uint16_t	lastModFileDate;
	uint32_t	crc32;
	uint32_t	compressedSize;
	uint32_t	uncompressedSize;
	uint16_t	filenameLength;
	uint16_t	extraFieldLength;
	uint16_t	fileCommentLength;
	uint16_t	diskNumberStart;
	uint16_t	internalFileAttributes;
	uint32_t	externalFileAttributes;
	uint32_t	relativeOffsetOfLocalHeader;
	// fileName ( variable length )
	// extraField ( variable length )
	// fileComment ( variable length )
} PACK_STRUCT zipFileCentralDirFileHeader_t;

typedef struct zipFileCentralDirEnd_s {
	uint32_t	sig;			// 'PK0506' end_of central dir signature			// (0x06054b50)
	uint16_t	numberDisk;		// number of this disk
	uint16_t	numberStart;	// number of the disk with the start of the central directory
	uint16_t	totalDisk;		// total number of entries in the central dir on this disk
	uint16_t	totalEntries;	// total number of entries in the central dir
	uint32_t	size;			// size of the central directory
	uint32_t	offset;			// offset of start of centraldirectory with respect to the starting disk number
	uint16_t	commentLength;	// zipfile comment length
	// zipFileComment ( variable length )
} PACK_STRUCT zipFileCentralDirEnd_t;

typedef struct zipFileExtraHeader_s {
	int16_t	id;
	int16_t	size;
} PACK_STRUCT zipFileExtraHeader_t;

#include "byteUnpack.h"

typedef struct zipFileEntry_s {
	size_t			offset;
	zipFileHeader_t	header;
} zipFileEntry_t;

xiZipLoader::xiZipLoader( xiFileSystem * const fs ) {
	fileSystem = fs;
}

xiZipLoader::~xiZipLoader() {
}

bool xiZipLoader::IsLoadableFileName( const char * const fileName ) const {
	return PathUtil::HasFileExtension( fileName, "zip", "pk3" );
}

bool xiZipLoader::IsLoadableReadFile( xiReadFile * const readFile ) const {
	zipFileHeader_t header;

	readFile->Read( &header.sig, sizeof( header.sig ) );

#if defined( __POSIX__ )
	header.sig = ntohl( header.sig );
#endif

	return ( header.sig == 0x04034b50 ); // Zip
}

xiFileArchive * xiZipLoader::CreateArchiveFromFileName( const char * const fileName, const xiFileList::ignoreParam_e ignoreMask ) const {
	xiFileArchive * archive = nullptr;
	xiReadFile * const file = fileSystem->CreateAndOpenFile( fileName );

	if ( file ) {
		archive = CreateArchiveFromReadFile( file, ignoreMask );
		file->Release();
	}

	return archive;
}

xiFileArchive * xiZipLoader::CreateArchiveFromReadFile( xiReadFile * const readFile, const xiFileList::ignoreParam_e ignoreMask ) const {
	xiFileArchive * archive = nullptr;
	if ( readFile ) {
		readFile->Seek( 0 );

		uint16_t sig;
		readFile->Read( &sig, 2 );

#if defined( __POSIX__ )
		sig = ntohs( sig );
#endif
		
		readFile->Seek( 0 );
		
		archive = new xiZipReader( readFile, ignoreMask );
	}

	return archive;
}

xiZipReader::xiZipReader( xiReadFile * const file, const xiFileList::ignoreParam_e ignoreMask ) : xiFileList( ( file ? file->GetFileName() : "" ), ignoreMask ) {
	readFile = file;
	fileInfo.entries = 0;
	fileInfo.entriesLen = 0;

	if ( readFile ) {
		readFile->Retain();

		// load file entries
		while ( ScanZipHeader( false ) ) {};

		Sort();
	}
}

xiZipReader::~xiZipReader() {
	if ( readFile ) {
		readFile->Release();
	}

	FileInfo_Clear();
}

xiReadFile * xiZipReader::CreateAndOpenFileName( const char * const fileName ) {
	const ptrdiff_t index = FindFile( fileName, ITEM_FILE );

	if ( index != -1 ) {
		return CreateAndOpenFileIndex( index );
	}

	return 0;
}

const xiFileList * xiZipReader::GetFileList() const {
	return this;
}

bool xiZipReader::ScanZipHeader( const bool ignoreGPBits ) {
	char zipFileName[FILE_NAME_LEN];
	zipFileName[0] = 0;

	zipFileEntry_t entry;
	entry.offset = 0;
	memset( &entry.header, 0, sizeof( zipFileHeader_t ) );

	readFile->Read( &entry.header, sizeof( zipFileHeader_t ) );

/*#if defined( __POSIX__ )
	entry.header.sig = ntohl( entry.header.sig );
	entry.header.versionToExtract = ntohs( entry.header.versionToExtract );
	entry.header.generalBitFlag = ntohs( entry.header.generalBitFlag );
	entry.header.compressionMethod = ntohs( entry.header.compressionMethod );
	entry.header.lastModFileTime = ntohs( entry.header.lastModFileTime );
	entry.header.lastModFileDate = ntohs( entry.header.lastModFileDate );
	entry.header.dataDescriptor.crc32 = ntohl( entry.header.dataDescriptor.crc32 );
	entry.header.dataDescriptor.compressedSize = ntohl( entry.header.dataDescriptor.compressedSize );
	entry.header.dataDescriptor.uncompressedSize = ntohl( entry.header.dataDescriptor.uncompressedSize );
	entry.header.fileNameLength = ntohs( entry.header.fileNameLength );
	entry.header.extraFieldLength = ntohs( entry.header.extraFieldLength );
#endif*/

	if ( entry.header.sig != 0x04034b50 ) {
		return false; // local file headers end here.
	}

	// read filename
	{
		char * tmp = new char[ entry.header.fileNameLength + 2 ];
		readFile->Read( tmp, entry.header.fileNameLength );
		tmp[entry.header.fileNameLength] = 0;

		PathUtil::SafeStrCopy( &zipFileName[0], tmp, FILE_NAME_LEN );

		delete [] tmp;
	}

	if ( entry.header.extraFieldLength ) {
		readFile->Seek( entry.header.extraFieldLength, xiFile::SEEK_RELATIVE );
	}

	// if bit 3 was set, use CentralDirectory for setup
	if ( !ignoreGPBits && entry.header.generalBitFlag & ZIP_INFO_IN_DATA_DESCRIPTOR ) {
		zipFileCentralDirEnd_t dirEnd;

		FileInfo_Clear();
		Files_Clear();

		// First place where the end record could be stored
		readFile->Seek( readFile->GetSize() - 22 );
		const char endID[] = { 0x50, 0x4b, 0x05, 0x06, 0x0 };
		char tmp[5] = { '\0' };
		bool found = false;
		// search for the end record ID
		while ( !found && readFile->GetPos() > 0 ) {
			ptrdiff_t seek = 8;
			readFile->Read( tmp, sizeof( int32_t ) );
			switch ( tmp[0] ) {
			case 0x50:
				if ( !strcmp( endID, tmp ) ) {
					seek = sizeof( int32_t );
					found = true;
				}
				break;
			case 0x4b:
				seek=5;
				break;
			case 0x05:
				seek=6;
				break;
			case 0x06:
				seek=7;
				break;
			}
			readFile->Seek( -seek, xiFile::SEEK_RELATIVE );
		}
		readFile->Read( &dirEnd, sizeof( dirEnd ) );

/*#if defined( __POSIX__ )
		dirEnd.numberDisk = ntohs( dirEnd.numberDisk );
		dirEnd.numberStart = ntohs( dirEnd.numberStart );
		dirEnd.totalDisk = ntohs( dirEnd.totalDisk );
		dirEnd.totalEntries = ntohs( dirEnd.totalEntries );
		dirEnd.size = ntohl( dirEnd.size );
		dirEnd.offset = ntohl( dirEnd.offset );
		dirEnd.commentLength = ntohs( dirEnd.commentLength );
#endif*/
		zipFileEntry_t * const biggerEntries = ( zipFileEntry_t * )realloc( fileInfo.entries, sizeof( *fileInfo.entries ) * dirEnd.totalEntries );
		if ( biggerEntries ) {
			fileInfo.entries = biggerEntries;
		}

		//FileInfo.reallocate(dirEnd.TotalEntries);
		readFile->Seek( ( ptrdiff_t )dirEnd.offset );
		while ( ScanCentralDirectoryHeader() ) {};

		return false;
	}

	// store position in file
	entry.offset = readFile->GetPos();
	// move forward length of data
	readFile->Seek( ( ptrdiff_t )entry.header.dataDescriptor.compressedSize, xiFile::SEEK_RELATIVE );

	const itemType_e type = ( zipFileName[strlen( zipFileName ) - 1] == '/' ? ITEM_DIRECTORY : ITEM_FILE );
	AddItem( zipFileName, entry.offset, ( size_t )entry.header.dataDescriptor.uncompressedSize, type, fileInfo.entriesLen );
	
	FileInfo_PushBack( &entry );

	return true;
}

bool xiZipReader::ScanCentralDirectoryHeader() {
	char zipFileName[FILE_NAME_LEN];
	zipFileName[0] = 0;

	zipFileCentralDirFileHeader_t entry;
	readFile->Read( &entry, sizeof( zipFileCentralDirFileHeader_t ) );
	
/*#if defined( __POSIX__ )
	entry.sig = ntohl( entry.sig );
	entry.versionMadeBy = ntohs( entry.versionMadeBy );
	entry.versionToExtract = ntohs( entry.versionToExtract );
	entry.generalBitFlag = ntohs( entry.generalBitFlag );
	entry.compressionMethod = ntohs( entry.compressionMethod );
	entry.lastModFileTime = ntohs( entry.lastModFileTime );
	entry.lastModFileDate = ntohs( entry.lastModFileDate );
	entry.crc32 = ntohl( entry.crc32 );
	entry.compressedSize = ntohl( entry.compressedSize );
	entry.uncompressedSize = ntohl( entry.uncompressedSize );
	entry.filenameLength = ntohs( entry.filenameLength );
	entry.extraFieldLength = ntohs( entry.extraFieldLength );
	entry.fileCommentLength = ntohs( entry.fileCommentLength );
	entry.diskNumberStart = ntohs( entry.diskNumberStart );
	entry.internalFileAttributes = ntohs( entry.internalFileAttributes );
	entry.externalFileAttributes = ntohl( entry.externalFileAttributes );
	entry.relativeOffsetOfLocalHeader = ntohl( entry.relativeOffsetOfLocalHeader );
#endif*/

	if ( entry.sig != 0x02014b50 ) {
		return false; // central dir headers end here.
	}

	const size_t pos = readFile->GetPos();
	readFile->Seek( ( ptrdiff_t )entry.relativeOffsetOfLocalHeader );
	ScanZipHeader( true );

	readFile->Seek( pos + entry.filenameLength + entry.extraFieldLength + entry.fileCommentLength );

	

	fileInfo.entries[fileInfo.entriesLen - 1].header.dataDescriptor.compressedSize = entry.compressedSize;
	fileInfo.entries[fileInfo.entriesLen - 1].header.dataDescriptor.uncompressedSize = entry.uncompressedSize;
	fileInfo.entries[fileInfo.entriesLen - 1].header.dataDescriptor.crc32 = entry.crc32;

	fileList[fileListLen - 1].size = ( size_t )entry.uncompressedSize;

	return true;
}

void xiZipReader::FileInfo_PushBack( zipFileEntry_t * const entry ) {
	const size_t biggerEntriesLen = fileInfo.entriesLen + 1;
	zipFileEntry_t * const biggerEntries = ( zipFileEntry_t * )realloc( fileInfo.entries, sizeof( *biggerEntries ) * biggerEntriesLen );
	
	if ( biggerEntries ) {
		memcpy( &biggerEntries[fileInfo.entriesLen], entry, sizeof( *entry ) );
		
		fileInfo.entries = biggerEntries;
		fileInfo.entriesLen = biggerEntriesLen;
	}
}

void xiZipReader::FileInfo_Clear() {
	if ( fileInfo.entries ) {
		free( fileInfo.entries );
		fileInfo.entriesLen = 0;
	}
}

void xiZipReader::Files_Clear() {
	if ( fileList ) {
		free( fileList );
		fileListLen = 0;
	}
}

xiReadFile * xiZipReader::CreateAndOpenFileIndex( const size_t fileIndex ) {
	const zipFileEntry_t * const e = &fileInfo.entries[fileList[fileIndex].ID];

	int16_t actualCompressionMethod = e->header.compressionMethod;

	xiReadFile * decrypted = nullptr;
	char * decryptedBuf = 0;
	const size_t decryptedSize = ( size_t )e->header.dataDescriptor.compressedSize;

	switch( actualCompressionMethod ) {
	case 0: // no compression
		{
			if ( decrypted ) {
				return decrypted;
			} else {
				return xiReadFile::CreateLimitReadFile( fileList[fileIndex].fullName, readFile, e->offset, decryptedSize );
			}
		}
	case 8:
		{

			const size_t uncompressedSize = ( size_t )e->header.dataDescriptor.uncompressedSize;
			char * const pBuf = new char[uncompressedSize];
			
			char * pcData = decryptedBuf;
			if ( !pcData ) {
				pcData = new char[decryptedSize];

				//memset(pcData, 0, decryptedSize);
				readFile->Seek( e->offset );
				readFile->Read( pcData, decryptedSize );
			}

			// Setup the inflate stream.
			z_stream stream;
			int32_t err;

			stream.next_in = ( Bytef * )pcData;
			stream.avail_in = ( uInt )decryptedSize;
			stream.next_out = ( Bytef * )pBuf;
			stream.avail_out = ( uInt )uncompressedSize;
			stream.zalloc = ( alloc_func )0;
			stream.zfree = ( free_func )0;

			// Perform inflation. wbits < 0 indicates no zlib header inside the data.
			err = inflateInit2(&stream, -MAX_WBITS);
			if (err == Z_OK)
			{
				err = inflate(&stream, Z_FINISH);
				inflateEnd(&stream);
				if (err == Z_STREAM_END)
					err = Z_OK;
				err = Z_OK;
				inflateEnd(&stream);
			}

			if (decrypted)
				delete( decrypted );
			else
				delete[] pcData;

			if (err != Z_OK)
			{
				//swprintf ( buf, 64, L"Error decompressing %s", Files[index].FullName.c_str() );
				//os::Printer::log( buf, ELL_ERROR);
				delete [] pBuf;
				return 0;
			}
			else {
				return xiReadFile::CreateMemoryReadFile( pBuf, uncompressedSize, fileList[fileIndex].fullName, true );
			}

		}
	case 12:
	case 14:
	case 99:
	default:
		return nullptr;
	};
}
