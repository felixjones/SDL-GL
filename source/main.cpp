#include "GL_Context.h"
#include "PNG_Texture.h"
#include "ReadFile.h"
#include "Shader.h"
#include "GL_Vertex.h"
#include "FileSystem.h"

extern "C" {
	#include "Matrix.h"
}

xiGLVertex_t shape[] = {
	{ { -0.75f, 0.75f, 0.0f }, {}, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
	{ { 0.75f, 0.75f, 0.0f }, {}, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
	{ { 0.75f, -0.75f, 0.0f }, {}, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
	{ { -0.75f, -0.75f, 0.0f }, {}, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }
};

xiGLVertex_t cube[] = {
	{ { -1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
	{ {  1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
	{ {  1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
	{ { -1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
	{ {  1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
	{ { -1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
	
	{ {  1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
	{ { -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
	{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
	{ {  1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
	{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
	{ {  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }
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
		GLContext_OpenFullScreenWithAA( context, 1920, 1080, 0 );
		//GLContext_SetVSync( VSYNC_ENABLE | VSYNC_DOUBLE_BUFFERED );

		xiShader * const shader = xiShader::Get();
		
		xiPNGTexture_t * const texture = PNGTexture_Alloc();

		xiReadFile * const pngFile = xiReadFile::CreateReadFile( "the_cage.png" );
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
		glGenBuffers( 2, &vbo[0] );
		{
			glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
			glBufferData( GL_ARRAY_BUFFER, 4 * sizeof( xiGLVertex_t ), shape, GL_STATIC_DRAW );

			glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
			glBufferData( GL_ARRAY_BUFFER, 12 * sizeof( xiGLVertex_t ), cube, GL_STATIC_DRAW );
		}
		
		GLuint vao;
		{
			glGenVertexArrays( 1, &vao );
		
			glBindVertexArray( vao );
			glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
			
			//glBindVertexArray( vao );
			//glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
			
			glEnableVertexAttribArray( 0 );
			glEnableVertexAttribArray( 1 );
			glEnableVertexAttribArray( 2 );
			glEnableVertexAttribArray( 3 );

			GLVertex_SetupAttributes();

			//glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 5, 0 );
			//glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 5, ( void * )( sizeof( float ) * 3 ) );
		
			//glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
		}
				
		GLuint textureID = PNGTexture_GLHandle( texture );

		glUseProgram( shaderProg );

		glProgramUniform1i( shaderProg, glGetUniformLocation( shaderProg, "textureSample" ), 0 );
		const int shadeLoc = glGetUniformLocation( shaderProg, "shade" );
		const int matrixLoc = glGetUniformLocation( shaderProg, "uniform_matrix" );

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, textureID );

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		
		glBindVertexArray( vao );

		float col = 0.0f;

		float accel = 0.0f;
		float dir = 1.0f;

		mat4_t viewMatrix;
		vec3_t eyePos = { 0.0f, 0.0f, -1.0f };
		vec3_t targetPos = { 0.0f, 0.0f, 0.0f };

		Matrix_LookAt( &viewMatrix, &eyePos, &targetPos, 0 );

		mat4_t modelMatrix = {
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};

		mat4_t projectionMatrix;
		Matrix_Perspective( &projectionMatrix, 90.0f, 640.0f / 480.0f, 0.1f, 100.0f );
		//Matrix_Orthographic( &projectionMatrix, -1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f );

		mat4_t viewProjectionMatrix;
		Matrix_Mul( &viewProjectionMatrix, &viewMatrix, &projectionMatrix, MAT4 );

		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );
		glDisable( GL_CULL_FACE );

		while ( GLContext_Run( context ) ) {
			Matrix_Identity( &modelMatrix );

			mat3_t rotationMatrix;
			vec3_t rotationVec = { col, col * 2.0f, col * 3.0f };
			Matrix_RotationDegree( &rotationMatrix, &rotationVec, VEC3 );

			Matrix_Mul( &modelMatrix, &rotationMatrix, &modelMatrix, MAT3 );

			mat4_t modelViewProjectionMatrix;
			Matrix_Mul( &modelViewProjectionMatrix, &modelMatrix, &viewProjectionMatrix, MAT4 );

			glUniformMatrix4fv( matrixLoc, 1, GL_FALSE, ( const GLfloat * )&modelViewProjectionMatrix );

			// wipe the drawing surface clear
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			//glUseProgram( shaderProg );
			//glBindVertexArray( vao );

			glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
			//glDrawArrays( GL_TRIANGLES, 0, 12 );
			
			GLContext_SwapWindow( context );
			GLContext_DrainEvents();

			col += context->deltaTime * accel;
			accel += context->deltaTime * dir * 2.0f;

			if ( accel > 50.0f ) {
				dir = -2.0f;
			} else if ( accel < -4.0f ) {
				dir = 1.0f;
			}

			glUniform1f( shadeLoc, col );
		}

		glDeleteProgram( shaderProg );
		
		shader->Release();
		PNGTexture_Release( texture );

		GLContext_Release( context );
	}

	return 1;
}
