#ifndef __GL_CONTEXT_H__
#define __GL_CONTEXT_H__

#define GLEW_STATIC
#include <GL/glew.h>
#include "os.h"

#if defined( __WIN_API__ ) && defined( __RELEASE__ )
	#define main	WinMain
#endif

#define WINDOW_NAME_LEN	( 80 )

typedef enum vsyncOptions_e {
	VSYNC_DISABLE			= 0x0,
	VSYNC_ENABLE			= 0x1,
	VSYNC_DOUBLE_BUFFERED	= 0x2
} vsyncOptions_t;

/*
================================================================================

	OpenGL Context management class

================================================================================
*/
typedef struct xiGLContext_s {

	void *	nativeContext;
	int		status;

	struct window_s {
		void *	handle;
		int		width;
		int		height;
		char	name[WINDOW_NAME_LEN];
	} window;

	struct memory_s {
		int	references;
		int	isHeap;
	} memory;

} xiGLContext_t;

const char *	GLContext_GetStatus( const int status );
int				GLContext_SetGLVersion( const int versionMajor, const int versionMinor );
int				GLContext_SetVSync( const int options );

xiGLContext_t *	GLContext_Init( xiGLContext_t * const self );
int				GLContext_SetWindowSizeWithAA( xiGLContext_t * const self, const int width, const int height, const int samples );
int				GLContext_SetWindowSize( xiGLContext_t * const self, const int width, const int height );
void			GLContext_SetWindowName( xiGLContext_t * const self, const char * const name );
void			GLContext_SwapWindow( xiGLContext_t * const self );
void			GLContext_Terminate( xiGLContext_t * const self );
int				GLContext_PollEvents( xiGLContext_t * const self );

xiGLContext_t *	GLContext_Alloc();
void			GLContext_Dealloc( xiGLContext_t * const self );
void			GLContext_Retain( xiGLContext_t * const self );
void			GLContext_Release( xiGLContext_t * const self );

#endif