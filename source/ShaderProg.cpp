#include "ShaderProg.h"

#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#define __UNSAFE__
#include "os.h"
#include "WriteFile.h"

int	xiShaderProg::currentProgram = -1;

xiShaderProg::xiShaderProg() {
	shaders = 0;
	shadersLen = 0;
	program = -1;
	linked = false;
}

xiShaderProg::~xiShaderProg() {
	if ( linked ) {
		glDeleteProgram( program );
	}

	if ( shaders ) {
		free( shaders );
	}
}

bool xiShaderProg::Attach( const int shader ) {
	if ( linked ) {
		return false;
	}

	const int biggerShadersLen = shadersLen + 1;
	int * const biggerShaders = ( int * )realloc( shaders, sizeof( *biggerShaders ) * biggerShadersLen );
	if ( biggerShaders ) {
		biggerShaders[shadersLen] = shader;

		shaders = biggerShaders;
		shadersLen = biggerShadersLen;
	}

	return true;
}

bool xiShaderProg::Link() {
	if ( shadersLen == 0 ) {
		return false;
	}

	if ( linked ) {
		return true;
	}
	
	program = glCreateProgram();

	int index = shadersLen - 1;
	do {
		glAttachShader( program, shaders[index] );
	} while ( index-- );
	
	glLinkProgram( program );

	int status;
	glGetProgramiv( program, GL_LINK_STATUS, &status );
	if ( status == GL_TRUE ) {
		linked = true;
		return true;
	}

	glDeleteProgram( program );
	return false;
}

bool xiShaderProg::Use() {
	if ( !linked ) {
		return false;
	}

	if ( currentProgram == program ) {
		return true;
	}

	glUseProgram( program );

	currentProgram = program;

	return true;
}

int xiShaderProg::GetUniformLoc( const char * const searchName ) {
	if ( !linked ) {
		return -1;
	}

	return glGetUniformLocation( program, searchName );
}

bool xiShaderProg::SetUniformInt( const int loc, const int val ) {
	if ( !linked ) {
		return false;
	}
	
	glProgramUniform1i( program, loc, val );
	return true;
}

bool xiShaderProg::SetUniformUInt( const int loc, const unsigned int val ) {
	if ( !linked ) {
		return false;
	}
	
	glProgramUniform1ui( program, loc, val );
	return true;
}

bool xiShaderProg::SetUniformFloat( const int loc, const float val ) {
	if ( !linked ) {
		return false;
	}

	glProgramUniform1f( program, loc, val );
	return true;
}

bool xiShaderProg::SetUniformPtr( const int loc, const void * const ptr, const uniformType_e type, const int count ) {
	if ( !linked ) {
		return false;
	}

	const float * const floatPtr = ( float * )ptr;
	const int * const intPtr = ( int * )ptr;
	const unsigned int * const uintPtr = ( unsigned int * )ptr;

	switch ( type ) {
	case UNIFORM_FLOAT:
		glProgramUniform1fv( program, loc, count, floatPtr );
		return true;
	case UNIFORM_VEC2:
		glProgramUniform2fv( program, loc, count, floatPtr );
		return true;
	case UNIFORM_VEC3:
		glProgramUniform3fv( program, loc, count, floatPtr );
		return true;
	case UNIFORM_VEC4:
		glProgramUniform4fv( program, loc, count, floatPtr );
		return true;
	case UNIFORM_INT:
		glProgramUniform1iv( program, loc, count, intPtr );
		return true;
	case UNIFORM_INT2:
		glProgramUniform2iv( program, loc, count, intPtr );
		return true;
	case UNIFORM_INT3:
		glProgramUniform3iv( program, loc, count, intPtr );
		return true;
	case UNIFORM_INT4:
		glProgramUniform4iv( program, loc, count, intPtr );
		return true;
	case UNIFORM_UINT:
		glProgramUniform1uiv( program, loc, count, uintPtr );
		return true;
	case UNIFORM_UINT2:
		glProgramUniform2uiv( program, loc, count, uintPtr );
		return true;
	case UNIFORM_UINT3:
		glProgramUniform3uiv( program, loc, count, uintPtr );
		return true;
	case UNIFORM_UINT4:
		glProgramUniform4uiv( program, loc, count, uintPtr );
		return true;
	case UNIFORM_MATRIX_2_2:
		glProgramUniformMatrix2fv( program, loc, count, false, floatPtr );
		return true;
	case UNIFORM_MATRIX_3_3:
		glProgramUniformMatrix3fv( program, loc, count, false, floatPtr );
		return true;
	case UNIFORM_MATRIX_4_4:
		glProgramUniformMatrix4fv( program, loc, count, false, floatPtr );
		return true;
	case UNIFORM_MATRIX_2_3:
		glProgramUniformMatrix2x3fv( program, loc, count, false, floatPtr );
		return true;
	case UNIFORM_MATRIX_3_2:
		glProgramUniformMatrix3x2fv( program, loc, count, false, floatPtr );
		return true;
	case UNIFORM_MATRIX_2_4:
		glProgramUniformMatrix2x4fv( program, loc, count, false, floatPtr );
		return true;		
	case UNIFORM_MATRIX_4_2:
		glProgramUniformMatrix4x2fv( program, loc, count, false, floatPtr );
		return true;
	case UNIFORM_MATRIX_3_4:
		glProgramUniformMatrix3x4fv( program, loc, count, false, floatPtr );
		return true;
	case UNIFORM_MATRIX_4_3:
		glProgramUniformMatrix4x3fv( program, loc, count, false, floatPtr );
		return true;
	}

	return false;
}