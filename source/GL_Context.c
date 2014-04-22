#include "GL_Context.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <string.h>
#include <stdio.h>

#if defined( __WIN_API__ )

	#pragma comment( lib, "opengl32" )
	#pragma comment( lib, "glew32s" )
	#pragma comment( lib, "SDL2" )

	#if defined( __X32__ ) && defined( __RELEASE__ )
		#pragma comment( lib, "SDL2main" )
	#endif

#endif

#define NULL_PTR	( ( xiGLContext_t * )0 )

enum statusGLContext_e {
	STATUS_OKAY,
	STATUS_ERROR_SDL_INIT,
	STATUS_COULD_NOT_CREATE_WINDOW,
	STATUS_TERMINATED,
	STATUS_ERROR_GLEW_INIT
};

static int sdlReferences = 0;
static int glewInitialised = 0;

/*
====================
GLContext_GetStatus

	Returns a string from a given status integer
====================
*/
const char * GLContext_GetStatus( const int status ) {
	switch ( status ) {
	case STATUS_OKAY:
		return "Valid";
	case STATUS_ERROR_SDL_INIT:
		return "Error initialising SDL";
	case STATUS_COULD_NOT_CREATE_WINDOW:
		return "Could not create window";
	case STATUS_TERMINATED:
		return "Terminated";
	case STATUS_ERROR_GLEW_INIT:
		return "Error initialising GLEW";
	}

	return "Unknown";
}

/*
====================
GLContext_SetGLVersion

	Sets the GL version
====================
*/
int GLContext_SetGLVersion( const int versionMajor, const int versionMinor ) {
	if ( !sdlReferences ) {
		return 0;
	}

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, versionMajor );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, versionMinor );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	
	return 1;
}

/*
====================
GLContext_SetVSync

	Sets the VSYNC options
====================
*/
int GLContext_SetVSync( const int options ) {
	if ( !sdlReferences ) {
		return 0;
	}

	if ( options & VSYNC_ENABLE ) {
		SDL_GL_SetSwapInterval( 1 );
	} else {
		SDL_GL_SetSwapInterval( 0 );
	}
	
	if ( options & VSYNC_DOUBLE_BUFFERED ) {
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	} else {
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 0 );
	}
		
	return 1;
}

/*
====================
GLContext_Init

	Initialises a context structure
====================
*/
xiGLContext_t *	GLContext_Init( xiGLContext_t * const self ) {
	if ( !self ) {
		return NULL_PTR;
	}

	self->status = STATUS_OKAY;
	self->window.handle = 0;
	self->nativeContext = 0;
	self->window.width = 0;
	self->window.height = 0;
	memset( &self->window.name[0], 0, sizeof( char ) * WINDOW_NAME_LEN );
	
	if ( !sdlReferences ) {
		if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
			self->status = STATUS_ERROR_SDL_INIT;
			return NULL_PTR;
		}
	}
	sdlReferences++;
	
	return self;
}

/*
====================
GLContext_SetWindowSizeWithAA

	Sets the size of the context window
	Also has an option for MSAA samples
====================
*/
int GLContext_SetWindowSizeWithAA( xiGLContext_t * const self, const int width, const int height, const int samples ) {
	if ( !sdlReferences ) {
		return 0;
	}
	
	if ( samples ) {
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, ( samples > 8 ? 8 : samples ) );

		glEnable( GL_MULTISAMPLE );

		glEnable( GL_LINE_SMOOTH );
		glEnable( GL_POLYGON_SMOOTH );

	} else {
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 0 );
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 0 );

		glDisable( GL_MULTISAMPLE );

		glDisable( GL_LINE_SMOOTH );
		glDisable( GL_POLYGON_SMOOTH );

	}
	
	return GLContext_SetWindowSize( self, width, height );
}

/*
====================
GLContext_SetWindowSize

	Sets the size of the context window
====================
*/
int GLContext_SetWindowSize( xiGLContext_t * const self, const int width, const int height ) {
	if ( self->status != STATUS_OKAY ) {
		return 0;
	}

	if ( self->window.handle ) {

		SDL_GL_DeleteContext( self->nativeContext );
		SDL_DestroyWindow( ( SDL_Window * )self->window.handle );

		self->window.handle = 0;
		self->nativeContext = 0;

	} 
	
	self->window.handle = SDL_CreateWindow( "", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

	if ( !self->window.handle ) {
		self->status = STATUS_COULD_NOT_CREATE_WINDOW;
		return 0;
	} else {
		self->window.width = width;
		self->window.height = height;
	
		self->nativeContext = SDL_GL_CreateContext( ( SDL_Window * )self->window.handle );
		
		if ( self->window.name[0] ) {
			SDL_SetWindowTitle( ( SDL_Window * )self->window.handle, &self->window.name[0] );
		}
	}

	glewExperimental = GL_TRUE;
	if ( !glewInitialised ) {
		GLenum status = glewInit();
		if ( status != GLEW_OK ) {
			self->status = STATUS_ERROR_GLEW_INIT;
			return 0;
		}

		 printf("----------------------------------------------------------------\n");
  printf("Graphics Successfully Initialized\n");
  printf("OpenGL Info\n");
  printf("    Version: %s\n", glGetString(GL_VERSION));
  printf("     Vendor: %s\n", glGetString(GL_VENDOR));
  printf("   Renderer: %s\n", glGetString(GL_RENDERER));
  printf("    Shading: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  printf("----------------------------------------------------------------\n");
	}
	
	return 1;
}

/*
====================
GLContext_SetWindowName

	Sets the name of the context window
====================
*/
void GLContext_SetWindowName( xiGLContext_t * const self, const char * const name ) {
	size_t charIndex = 0;
	
	if ( self->status != STATUS_OKAY ) {
		return;
	}

	charIndex = strlen( name ) + 1;

	if ( charIndex > WINDOW_NAME_LEN ) {
		charIndex = WINDOW_NAME_LEN;
	}

	memcpy( &self->window.name[0], &name[0], sizeof( char ) * charIndex );

	if ( self->window.handle ) {
		SDL_SetWindowTitle( ( SDL_Window * )self->window.handle, &self->window.name[0] );
	}
}

/*
====================
GLContext_SwapWindow

	Swaps the SDL window for drawing everything
====================
*/
void GLContext_SwapWindow( xiGLContext_t * const self ) {
	if ( self->status != STATUS_OKAY ) {
		return;
	}
	
	if ( self->window.handle ) {
		SDL_GL_SwapWindow( ( SDL_Window * )self->window.handle );
	}
}

/*
====================
GLContext_PollEvents

	Handles the event system via SDL
====================
*/
int GLContext_PollEvents( xiGLContext_t * const self ) {
	SDL_Event sdlEvent;
	int pollState = 1;

	if ( self->status != STATUS_OKAY ) {
		return 0;
	}
	
	while ( SDL_PollEvent( &sdlEvent ) ) {
		if ( sdlEvent.type == SDL_WINDOWEVENT && sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE ) {
			pollState = 0;
		}
	}

	return pollState;
}

/*
====================
GLContext_Terminate

	Closes context
====================
*/
void GLContext_Terminate( xiGLContext_t * const self ) {
	if ( self->status == STATUS_OKAY ) {
		SDL_GL_DeleteContext( self->nativeContext );
		SDL_DestroyWindow( ( SDL_Window * )self->window.handle );
	}

	sdlReferences--;
	if ( !sdlReferences ) {
		SDL_Quit();
	}

	self->status = STATUS_TERMINATED;
}

xiGLContext_t * GLContext_Alloc() {
	xiGLContext_t * const self = ( xiGLContext_t * )malloc( sizeof( xiGLContext_t ) );

	if ( self ) {
		memset( self, 0, sizeof( xiGLContext_t ) );

		self->memory.isHeap = 1;
		GLContext_Retain( self );
	}
	
	return self;
}

void GLContext_Dealloc( xiGLContext_t * const self ) {
	GLContext_Terminate( self );
	free( self );
}

void GLContext_Retain( xiGLContext_t * const self ) {
	if ( self->memory.isHeap ) {
		self->memory.references++;
	}
}

void GLContext_Release( xiGLContext_t * const self ) {
	if ( self->memory.isHeap ) {
		self->memory.references--;

		if ( !self->memory.references ) {
			GLContext_Dealloc( self );
		}
	}	
}