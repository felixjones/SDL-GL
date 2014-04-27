#include "Shader.h"

#include <stdlib.h>
#include <string.h>
#define GLEW_STATIC
#include <GL/glew.h>
#define __UNSAFE__
#include "os.h"
#include "ReadFile.h"
#include "WriteFile.h"

#define MIN( X, Y )	( X < Y ? X : Y )

xiShader *	xiShader::singleton = nullptr;
int32_t				xiShader::references = 0;

int xiShader::Shaders_Compare( const void * const left, const void * const right ) {
	const shader_t * const leftShader = ( shader_t * )left;
	const shader_t * const rightShader = ( shader_t * )right;

	const size_t leftLen = strlen( leftShader->name );
	const size_t rightLen = strlen( rightShader->name );
	const size_t minLen = MIN( leftLen, rightLen );
	
	for ( size_t ii = 0; ii < minLen; ii++ ) {
		if ( leftShader->name[ii] < rightShader->name[ii] ) {
			return -1;
		} else if ( leftShader->name[ii] > rightShader->name[ii] ) {
			return 1;
		}
	}

	if ( leftLen < rightLen ) {
		return -1;
	} else if ( leftLen > rightLen ) {
		return 1;
	}

	return 0;
}

xiShader * xiShader::Get() {
	if ( !singleton ) {
		singleton = new xiShader();
	}

	singleton->Retain();

	return singleton;
}

void xiShader::Retain() {
	references++;
}

void xiShader::Release() {
	references--;

	if ( !references ) {
		delete( this );
		singleton = nullptr;
	}
}

xiShader::xiShader() {
	shaders.ptr = 0;
	shaders.len = 0;
}

xiShader::~xiShader() {
	if ( shaders.ptr ) {
		int32_t index = shaders.len - 1;
		do {
			glDeleteShader( shaders.ptr[index].glHandle );
		} while ( index-- );

		free( shaders.ptr );
	}
}

int xiShader::Compile( const char * const fileName, const shaderType_e type ) {
	shader_t * const shader = Shaders_Find( fileName );
	
	if ( shader ) {
		if ( shader->type != type ) {
			// Identical names
			return -1;
		}

		return shader->glHandle;
	}

	GLuint shaderType = 0;
	switch ( type ) {
	case SHADER_COMPUTE:
		shaderType = GL_COMPUTE_SHADER;
		break;
	case SHADER_VERTEX:
		shaderType = GL_VERTEX_SHADER;
		break;
	case SHADER_TESS_CONTROL:
		shaderType = GL_TESS_CONTROL_SHADER;
		break;
	case SHADER_TESS_EVALUATION:
		shaderType = GL_TESS_EVALUATION_SHADER;
		break;
	case SHADER_GEOMETRY:
		shaderType = GL_GEOMETRY_SHADER;
		break;
	case SHADER_FRAGMENT:
		shaderType = GL_FRAGMENT_SHADER;
		break;
	}

	if ( !shaderType ) {
		return -1;
	}

	// Load new shader
	xiReadFile * const readFile = xiReadFile::CreateReadFile( fileName );
	if ( readFile ) {
		const size_t sourceLen = readFile->GetSize();
		const GLint glSourceLen = ( GLint )sourceLen;
		
		char * const sourceBuffer = new char[sourceLen + 1];
		readFile->Read( sourceBuffer, sourceLen );
		sourceBuffer[sourceLen] = 0;

		readFile->Release();

		GLuint shaderHandle = glCreateShader( shaderType );
		glShaderSource( shaderHandle, 1, ( const char ** )&sourceBuffer, &glSourceLen );
		glCompileShader( shaderHandle );
		delete[]( sourceBuffer );
		
		GLint isCompiled = 0;
		glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &isCompiled );
		if ( isCompiled == GL_FALSE ) {
			GLint maxLength = 0;
			glGetShaderiv( shaderHandle, GL_INFO_LOG_LENGTH, &maxLength );
 
			char * const infoLog = new char[maxLength];
			glGetShaderInfoLog( shaderHandle, maxLength, &maxLength, &infoLog[0] );
 
			const size_t nameLen = strlen( fileName ) + 4;
			char * const logName = new char[nameLen + 1];
			strcpy( logName, fileName );
			logName[nameLen - 4] = '.';
			logName[nameLen - 3] = 'l';
			logName[nameLen - 2] = 'o';
			logName[nameLen - 1] = 'g';
			logName[nameLen] = 0;

			xiWriteFile * const writeFile = xiWriteFile::CreateWriteFile( logName );
			if ( writeFile ) {
				writeFile->Write( infoLog, maxLength );

				writeFile->Release();
			}
			delete[]( logName );
			delete[]( infoLog );
			
			glDeleteShader( shaderHandle );

			return -1;
		}

		shader_t newShader;
		newShader.glHandle = shaderHandle;
		newShader.type = type;
		strcpy( &newShader.name[0], fileName );

		Shaders_PushBack( &newShader );
		Shaders_Sort();

		return newShader.glHandle;
	}

	return -1;
}

void xiShader::Shaders_Sort() {
	qsort( shaders.ptr, shaders.len, sizeof( *shaders.ptr ), &Shaders_Compare );
}

xiShader::shader_t * xiShader::Shaders_Find( const char * const name ) {
	if ( !shaders.ptr ) {
		return nullptr;
	}
	
	shader_t search;
	strcpy( &search.name[0], name );

	int32_t boundLeft = 0;
	int32_t boundRight = shaders.len;
	
	while ( boundRight >= boundLeft ) {
		const int32_t cursor = ( boundLeft + boundRight ) >> 1;

		const int dir = Shaders_Compare( &shaders.ptr[cursor], &search );
		if ( dir == 0 ) {
			return &shaders.ptr[cursor];
		}

		if ( dir < 0 ) {
			boundLeft = cursor + 1;
		} else if ( dir > 0 ) {
			boundRight = cursor - 1;
		}
	}

	return nullptr;
}

void xiShader::Shaders_PushBack( shader_t * const newShader ) {
	const int32_t biggerLen = shaders.len + 1;
	shader_t * const biggerPtr = ( shader_t * )realloc( shaders.ptr, sizeof( *biggerPtr ) * biggerLen );
	
	if ( biggerPtr ) {
		memcpy( &biggerPtr[shaders.len], newShader, sizeof( *newShader ) );

		shaders.ptr = biggerPtr;
		shaders.len = biggerLen;
	}
}