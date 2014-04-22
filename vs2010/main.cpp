#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <SDL.h>

#include "os.h"

#if defined( __WIN_API__ )

	#pragma comment( lib, "opengl32" )
	#pragma comment( lib, "SDL2" )
	#pragma comment( lib, "glew32s" )

	#undef main

	#if !defined( _DEBUG )
		#define main	WinMain
	#endif

#endif

int main( int argc, char ** argv ) {
    SDL_Window * mainWindow;
    SDL_GLContext glContext;
 
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        // Could not Initialise video
		return 0;
	}
 
    /* Request opengl 3.0 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
 
    /* Turn on double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
 
    /* Create our window centered at 512x512 resolution */
    mainWindow = SDL_CreateWindow( "Tutorial1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    if ( !mainWindow ) {
		// Could not create window
		return 1;
	}
  
    /* Create our opengl context and attach it to our window */
	glContext = SDL_GL_CreateContext( mainWindow );
 
 
    /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    SDL_GL_SetSwapInterval( 1 );
 
    /* Clear our buffer with a red background */
    glClearColor ( 1.0, 0.0, 0.0, 1.0 );
    glClear ( GL_COLOR_BUFFER_BIT );
    /* Swap our back buffer to the front */
    SDL_GL_SwapWindow( mainWindow );
    /* Wait 2 seconds */
    SDL_Delay(2000);
 
    /* Same as above, but green */
    glClearColor ( 0.0, 1.0, 0.0, 1.0 );
    glClear ( GL_COLOR_BUFFER_BIT );
    SDL_GL_SwapWindow( mainWindow );
    SDL_Delay(2000);
 
    /* Same as above, but blue */
    glClearColor ( 0.0, 0.0, 1.0, 1.0 );
    glClear ( GL_COLOR_BUFFER_BIT );
    SDL_GL_SwapWindow( mainWindow );
    SDL_Delay(2000);
 
    /* Delete our opengl context, destroy our window, and shutdown SDL */
    SDL_GL_DeleteContext( glContext );
    SDL_DestroyWindow( mainWindow );
    SDL_Quit();
 
    return 0;
}