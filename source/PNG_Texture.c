#include "PNG_Texture.h"

#include <string.h>
#include <stdlib.h>
#include "png.h"
#include "pngstruct.h"

#define NULL_PTR	( ( xiPNGTexture_t * )0 )

typedef struct pngBytes_s {
	const char *	bytePtr;
	size_t			byteLen;
	size_t			pos;
} pngBytes_t;

static void PNGTexture_ReadBytes( png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead ) {
	pngBytes_t * const pngBytes = ( pngBytes_t * )png_ptr->io_ptr;

	if ( pngBytes == NULL ) {
		return;   // add custom error handling here
	}

	/*if( byteCountToRead != pngBytes->byteLen ) {
		return;   // add custom error handling here
	}*/

	memcpy( outBytes, &pngBytes->bytePtr[pngBytes->pos], byteCountToRead );
	pngBytes->pos += byteCountToRead;
}

GLuint PNGTexture_LoadBytes( xiPNGTexture_t * const self, const char * const bytes, const size_t byteLen ) {
	png_byte header[8];

	if ( byteLen < 8 ) {
		return 0;
	}
	
	if ( self->glHandle ) {
		glDeleteTextures( 1, &self->glHandle );
		self->glHandle = -1;
	}

	memcpy( &header[0], bytes, sizeof( png_byte ) * 8 );

	if ( png_sig_cmp( &header[0], 0, 8 ) == 0 ) {
		png_structp png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
		
		if ( png_ptr ) {
			png_infop info_ptr = png_create_info_struct( png_ptr );
			png_infop end_info = 0;
			size_t rowbytes = 0;
			png_bytep * row_pointers = 0;
			int ii = 0;
			GLuint texture = 0;
			GLint format = GL_RGBA;
			pngBytes_t pngBytes;
			png_byte * image_data = 0;
			int color_type;

			if ( !info_ptr ) {
				png_destroy_read_struct( &png_ptr, ( png_infopp )NULL, ( png_infopp )NULL );

				return 0;
			}

			end_info = png_create_info_struct( png_ptr );
			if ( !end_info ) {
				png_destroy_read_struct( &png_ptr, &info_ptr, ( png_infopp )NULL );

				return 0;
			}

			if ( setjmp( png_jmpbuf( png_ptr ) ) ) {
				png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );

				return 0;
			}

			pngBytes.bytePtr = bytes;
			pngBytes.byteLen = byteLen;
			pngBytes.pos = 0;

			png_set_read_fn( png_ptr, &pngBytes, &PNGTexture_ReadBytes );
			
			png_read_info( png_ptr, info_ptr );

			png_get_IHDR( png_ptr, info_ptr, ( png_uint_32 * )&self->width, ( png_uint_32 * )&self->height, &self->depth, &color_type, NULL, NULL, NULL );

			if ( color_type == PNG_COLOR_TYPE_RGB ) {
				format = GL_RGB;
				self->pixLen = 3;
			} else {
				self->pixLen = 4;
			}
			
			png_read_update_info( png_ptr, info_ptr );

			rowbytes = png_get_rowbytes( png_ptr, info_ptr );

			rowbytes += 3 - ( ( rowbytes - 1 ) % 4 );

			image_data = ( png_byte * )malloc( rowbytes * self->height * sizeof( png_byte ) + 15 );
			if ( image_data == NULL ) {
				png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );

				return 0;
			}

			row_pointers = ( png_bytep * )malloc( self->height * sizeof( png_bytep ) );
			if ( row_pointers == NULL ) {
				png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );
				free( image_data );

				return 0;
			}

			ii = self->height - 1;
			do {
				row_pointers[self->height - 1 - ii] = image_data + ii * rowbytes;
			} while ( ii-- );

			png_read_image( png_ptr, row_pointers );

			glGenTextures( 1, &texture );
			glBindTexture( GL_TEXTURE_2D, texture );

			glTexImage2D( GL_TEXTURE_2D, 0, format, self->width, self->height, 0, format, GL_UNSIGNED_BYTE, image_data );

			png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );
			free( image_data );
			free( row_pointers );

			self->glHandle = texture;
		}
	}

	return self->glHandle;
}

void PNGTexture_GLParam( const xiPNGTexture_t * const self, const GLenum param, const GLint arg ) {
	glBindTexture( GL_TEXTURE_2D, self->glHandle );
	glTexParameteri( GL_TEXTURE_2D, param, arg );
}

void PNGTexture_GLGenMips( const xiPNGTexture_t * const self ) {
	glBindTexture( GL_TEXTURE_2D, self->glHandle );
	glGenerateMipmap( GL_TEXTURE_2D );
}

xiPNGTexture_t * PNGTexture_InitWithBytes( xiPNGTexture_t * const self, const char * const bytes, const size_t byteLen ) {
	if ( !self ) {
		return NULL_PTR;
	}

	self->width = 0;
	self->height = 0;

	PNGTexture_LoadBytes( self, bytes, byteLen );

	return self;
}

void PNGTexture_Terminate( xiPNGTexture_t * const self ) {
	if ( self->glHandle ) {
		glDeleteTextures( 1, &self->glHandle );
	}
}

GLuint PNGTexture_GLHandle( const xiPNGTexture_t * const self ) {
	return self->glHandle;
}

xiPNGTexture_t * PNGTexture_Alloc() {
	xiPNGTexture_t * const self = ( xiPNGTexture_t * )malloc( sizeof( xiPNGTexture_t ) );

	if ( self ) {
		memset( self, 0, sizeof( xiPNGTexture_t ) );

		self->memory.isHeap = 1;
		PNGTexture_Retain( self );

		self->glHandle = -1;
	}
	
	return self;
}

void PNGTexture_Dealloc( xiPNGTexture_t * const self ) {
	PNGTexture_Terminate( self );
	free( self );
}

void PNGTexture_Retain( xiPNGTexture_t * const self ) {
	if ( self->memory.isHeap ) {
		self->memory.references++;
	}
}

void PNGTexture_Release( xiPNGTexture_t * const self ) {
	if ( self->memory.isHeap ) {
		self->memory.references--;

		if ( !self->memory.references ) {
			PNGTexture_Dealloc( self );
		}
	}
}
