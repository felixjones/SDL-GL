#include "GL_Context.h"
#include "PNG_Texture.h"
#include "ReadFile.h"

float points[] = {
	0.0f,  0.5f,  0.0f,
	0.5f, -0.5f,  0.0f,
	-0.5f, -0.5f,  0.0f
};

float uvs[] = {
	0.0f,  0.0f,
	1.0f,  0.0f,
	1.0f,  1.0f
};

int main( int argc, char ** argv ) {
	xiGLContext_t * const context = GLContext_Init( GLContext_Alloc() );

	if ( context ) {
		GLContext_SetWindowName( context, "OpenGL Test" );
		GLContext_SetGLVersion( 3, 3 );
		GLContext_OpenWindowWithAA( context, 800, 600, 8 );
		GLContext_SetVSync( VSYNC_ENABLE | VSYNC_DOUBLE_BUFFERED );
		
		xiPNGTexture_t * const texture = PNGTexture_Alloc();

		xiReadFile * const pngFile = xiReadFile::CreateReadFile( "image.png" );
		if ( pngFile ) {
			char * const pngBytes = new char[pngFile->GetSize()];
			pngFile->Read( pngBytes, pngFile->GetSize() );

			PNGTexture_InitWithBytes( texture, pngBytes, pngFile->GetSize() );

			delete[]( pngBytes );
		}
		
		char * vertex_shader = 0;
		xiReadFile * const vertexSourceFile = xiReadFile::CreateReadFile( "basic.vsh" );
		if ( vertexSourceFile ) {
			vertex_shader = new char[vertexSourceFile->GetSize() + 1];
			vertexSourceFile->Read( vertex_shader, vertexSourceFile->GetSize() );
			vertex_shader[vertexSourceFile->GetSize()] = 0;

			vertexSourceFile->Release();
		}
		
		char * fragment_shader = 0;
		xiReadFile * const fragmentSourceFile = xiReadFile::CreateReadFile( "basic.fsh" );
		if ( fragmentSourceFile ) {
			fragment_shader = new char[fragmentSourceFile->GetSize() + 1];
			fragmentSourceFile->Read( fragment_shader, fragmentSourceFile->GetSize() );
			fragment_shader[fragmentSourceFile->GetSize()] = 0;

			fragmentSourceFile->Release();
		}

		GLuint vs = glCreateShader( GL_VERTEX_SHADER );
		GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
		
		if ( vertex_shader ) {
			glShaderSource( vs, 1, ( const char ** )&vertex_shader, 0 );
			glCompileShader( vs );
			delete[]( vertex_shader );

			GLint isCompiled = 0;
			glGetShaderiv(vs, GL_COMPILE_STATUS, &isCompiled);
			if(isCompiled == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);
 
				//The maxLength includes the NULL character
				char * const infoLog = new char[maxLength];
				glGetShaderInfoLog(vs, maxLength, &maxLength, &infoLog[0]);
 
				//We don't need the shader anymore.
				glDeleteShader(vs);
				delete[]( infoLog );
			}
		}
		
		if ( fragment_shader ) {
			glShaderSource( fs, 1, ( const char ** )&fragment_shader, 0 );
			glCompileShader( fs );
			delete[]( fragment_shader );
			
			GLint isCompiled = 0;
			glGetShaderiv(fs, GL_COMPILE_STATUS, &isCompiled);
			if(isCompiled == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength);
 
				//The maxLength includes the NULL character
				char * const infoLog = new char[maxLength];
				glGetShaderInfoLog(fs, maxLength, &maxLength, &infoLog[0]);
 
				//We don't need the shader anymore.
				glDeleteShader(fs);
				delete[]( infoLog );
			}
		}
				
		GLuint shaderProg = glCreateProgram();

		glAttachShader( shaderProg, fs );
		glAttachShader( shaderProg, vs );
		glLinkProgram( shaderProg );
		
		glClearColor( 1.0, 0.0, 0.0, 1.0 );
		glClear( GL_COLOR_BUFFER_BIT );

		GLuint vbo[2];
		glGenBuffers( 2, &vbo[0] );
		{
			glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
			glBufferData( GL_ARRAY_BUFFER, 9 * sizeof( float ), points, GL_STATIC_DRAW );

			glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
			glBufferData( GL_ARRAY_BUFFER, 6 * sizeof( float ), uvs, GL_STATIC_DRAW );
		}
		
		GLuint vao;
		{
			glGenVertexArrays( 1, &vao );
			glBindVertexArray( vao );
		
			glEnableVertexAttribArray( 0 );
			glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
			glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
		
			glEnableVertexAttribArray( 1 );
			glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
			glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, 0 );
		}
				
		GLuint textureID = PNGTexture_GLHandle( texture );

		glUseProgram( shaderProg );
		GLint texUnitLoc = glGetUniformLocation( shaderProg, "textureSample" );
		glProgramUniform1i( shaderProg, texUnitLoc, 0 );

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, textureID );

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		while ( GLContext_Run( context ) ) {
			// wipe the drawing surface clear
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			glUseProgram( shaderProg );
			glBindVertexArray( vao );
			glDrawArrays( GL_TRIANGLES, 0, 3 );

			GLContext_SwapWindow( context );
			GLContext_DrainEvents();
		}
		
		PNGTexture_Release( texture );

		GLContext_Release( context );
	}

	return 1;
}
