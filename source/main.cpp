#include "GL_Context.h"
#include "PNG_Texture.h"
#include "ReadFile.h"
#include "Shader.h"
#include "GL_Vertex.h"

xiGLVertex_t shape[] = {
	{ { -0.5f, 0.5f, 0.5f }, {}, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
	{ { 0.5f, 0.5f, 0.5f }, {}, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
	{ { 0.5f, -0.5f, 0.5f }, {}, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
	{ { -0.5f, -0.5f, 0.5f }, {}, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
};

float points[] = {
	-0.5f,  0.5f,  0.0f,	0.0f, 1.0f,
	0.5f, 0.5f,  0.0f,		1.0f, 1.0f,
	0.5f, -0.5f,  0.0f,		1.0f, 0.0f,
	-0.5f, -0.5f,  0.0f,	0.0f, 0.0f
};

float uvs[] = {
	0.0f,  1.0f,
	1.0f,  1.0f,
	1.0f,  0.0f,
	0.0f,  0.0f
};

int main( int argc, char ** argv ) {
	xiGLContext_t * const context = GLContext_Init( GLContext_Alloc() );

	if ( context ) {
		GLContext_SetWindowName( context, "OpenGL Test" );
		GLContext_SetGLVersion( 3, 3 );
		GLContext_OpenWindowWithAA( context, 800, 600, 8 );
		GLContext_SetVSync( VSYNC_ENABLE | VSYNC_DOUBLE_BUFFERED );

		xiShader * const shader = xiShader::Get();
		
		xiPNGTexture_t * const texture = PNGTexture_Alloc();

		xiReadFile * const pngFile = xiReadFile::CreateReadFile( "image.png" );
		if ( pngFile ) {
			char * const pngBytes = new char[pngFile->GetSize()];
			pngFile->Read( pngBytes, pngFile->GetSize() );

			PNGTexture_InitWithBytes( texture, pngBytes, pngFile->GetSize() );
			
			PNGTexture_GLParam( texture, GL_TEXTURE_WRAP_S, GL_REPEAT );
			PNGTexture_GLParam( texture, GL_TEXTURE_WRAP_T, GL_REPEAT );
			PNGTexture_GLParam( texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			PNGTexture_GLParam( texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			PNGTexture_GLGenMips( texture );

			delete[]( pngBytes );
		}
		
		const GLuint vs = shader->Compile( "basic.vsh", xiShader::SHADER_VERTEX );
		const GLuint fs = shader->Compile( "basic.fsh", xiShader::SHADER_FRAGMENT );
		
		const int shaderProg = glCreateProgram();
		glAttachShader( shaderProg, vs );
		glAttachShader( shaderProg, fs );
		glLinkProgram( shaderProg );

		glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		GLuint vbo[2];
		glGenBuffers( 1, &vbo[0] );
		{
			glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
			glBufferData( GL_ARRAY_BUFFER, 4 * sizeof( xiGLVertex_t ), shape, GL_STATIC_DRAW );

			//glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
			//glBufferData( GL_ARRAY_BUFFER, 8 * sizeof( float ), uvs, GL_STATIC_DRAW );
		}
		
		GLuint vao;
		{
			glGenVertexArrays( 1, &vao );
			glBindVertexArray( vao );
		
			glEnableVertexAttribArray( 0 );
			glEnableVertexAttribArray( 1 );
			glEnableVertexAttribArray( 2 );
			glEnableVertexAttribArray( 3 );

			// layoutLocation, elementCount, elementType, willNormalise, skipBytes, firstOffsetBytes

			glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
			GLVertex_SetupAttributes();
			//glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 5, 0 );
			//glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 5, ( void * )( sizeof( float ) * 3 ) );
		
			//glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
		}
				
		GLuint textureID = PNGTexture_GLHandle( texture );

		glUseProgram( shaderProg );

		glProgramUniform1i( shaderProg, glGetUniformLocation( shaderProg, "textureSample" ), 0 );
		const int shadeLoc = glGetUniformLocation( shaderProg, "shade" );

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, textureID );

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		
		glBindVertexArray( vao );

		float col = 0.0f;

		float accel = 0.0f;
		float dir = 1.0f;

		while ( GLContext_Run( context ) ) {
			// wipe the drawing surface clear
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			//glUseProgram( shaderProg );
			//glBindVertexArray( vao );
			glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
			GLContext_SwapWindow( context );
			GLContext_DrainEvents();

			col += context->deltaTime * accel;
			accel += context->deltaTime * dir * 2.0f;

			if ( accel > 50.0f ) {
				dir = -2.0f;
			} else if ( accel < -4.0f ) {
				dir = 1.0f;
			}

			glProgramUniform1f( shaderProg, shadeLoc, col );
		}

		glDeleteProgram( shaderProg );
		
		shader->Release();
		PNGTexture_Release( texture );

		GLContext_Release( context );
	}

	return 1;
}
