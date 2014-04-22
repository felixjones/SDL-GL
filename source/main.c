#include "GL_Context.h"

float points[] = {
	0.0f,  0.5f,  0.0f,
	0.5f, -0.5f,  0.0f,
	-0.5f, -0.5f,  0.0f
};

const char* vertex_shader =
	"#version 130\n"
	"in vec3 vp;"
	"void main () {"
	"  gl_Position = vec4 (vp, 1.0);"
	"}";

const char* fragment_shader =
	"#version 130\n"
	"out vec4 frag_colour;"
	"void main () {"
	"  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
	"}";

GLuint vao;
GLuint vbo;

GLuint vs;
GLuint fs;
GLuint shaderProg;

int main( int argc, char ** argv ) {
	xiGLContext_t * const context = GLContext_Init( GLContext_Alloc() );
	GLContext_SetGLVersion( 3, 0 );
			
	if ( context ) {
		GLContext_SetWindowSizeWithAA( context, 800, 600, 8 );
		GLContext_SetVSync( VSYNC_ENABLE | VSYNC_DOUBLE_BUFFERED );

		vs = glCreateShader (GL_VERTEX_SHADER);
		fs = glCreateShader (GL_FRAGMENT_SHADER);

		glShaderSource (vs, 1, &vertex_shader, 0);
		glCompileShader (vs);

		glShaderSource (fs, 1, &fragment_shader, 0);
		glCompileShader (fs);
			
		shaderProg = glCreateProgram ();

		glAttachShader( shaderProg, fs );
		glAttachShader( shaderProg, vs );
		glLinkProgram( shaderProg );
		
		glClearColor( 1.0, 0.0, 0.0, 1.0 );
		glClear( GL_COLOR_BUFFER_BIT );

		vbo = 0;

		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (float), points, GL_STATIC_DRAW);

		glGenVertexArrays (1, &vao);
		glBindVertexArray (vao);
		glEnableVertexAttribArray (0);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		while ( GLContext_PollEvents( context ) ) {

			// wipe the drawing surface clear
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram( shaderProg );
			glBindVertexArray( vao );
			glDrawArrays( GL_TRIANGLES, 0, 3 );

			GLContext_SwapWindow( context );

		}

		GLContext_Release( context );
	}
}