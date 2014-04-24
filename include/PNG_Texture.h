#ifndef __PNG_TEXTURE_H__
#define __PNG_TEXTURE_H__

#if defined( __cplusplus )
extern "C" {
#endif

#include <stdint.h>
#define GLEW_STATIC
#include <GL/glew.h>

typedef struct xiPNGTexture_s {

	GLuint		glHandle;
	uint32_t	width;
	uint32_t	height;
	int			depth;
	int			pixLen;

	struct memory_s {
		int	references;
		int	isHeap;
	} memory;

} xiPNGTexture_t;

xiPNGTexture_t *	PNGTexture_InitWithBytes( xiPNGTexture_t * const self, const char * const bytes, const size_t byteLen );
void				PNGTexture_Terminate( xiPNGTexture_t * const self );
GLuint				PNGTexture_GLHandle( const xiPNGTexture_t * const self );
GLuint				PNGTexture_LoadBytes( xiPNGTexture_t * const self, const char * const bytes, const size_t byteLen );
void				PNGTexture_GLParam( const xiPNGTexture_t * const self, const GLenum param, const GLint arg );
void				PNGTexture_GLGenMips( const xiPNGTexture_t * const self );

xiPNGTexture_t *	PNGTexture_Alloc();
void				PNGTexture_Dealloc( xiPNGTexture_t * const self );
void				PNGTexture_Retain( xiPNGTexture_t * const self );
void				PNGTexture_Release( xiPNGTexture_t * const self );

#if defined( __cplusplus )
}
#endif

#endif