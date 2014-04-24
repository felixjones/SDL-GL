#ifndef __SHADER_PROG_H__
#define __SHADER_PROG_H__

class xiShaderProg {
public:
	enum uniformType_e {
		UNIFORM_FLOAT,
		UNIFORM_VEC2,
		UNIFORM_VEC3,
		UNIFORM_VEC4,
		
		UNIFORM_INT,
		UNIFORM_INT2,
		UNIFORM_INT3,
		UNIFORM_INT4,
		
		UNIFORM_UINT,
		UNIFORM_UINT2,
		UNIFORM_UINT3,
		UNIFORM_UINT4,

		UNIFORM_MATRIX_2_2,
		UNIFORM_MATRIX_3_3,
		UNIFORM_MATRIX_4_4,
		
		UNIFORM_MATRIX_2_3,
		UNIFORM_MATRIX_3_2,
		UNIFORM_MATRIX_2_4,
		
		UNIFORM_MATRIX_4_2,
		UNIFORM_MATRIX_3_4,
		UNIFORM_MATRIX_4_3
	};

			xiShaderProg();
			~xiShaderProg();
	bool	Attach( const int shader );
	bool	Link();
	bool	Use();
	int		GetUniformLoc( const char * const searchName );
	bool	SetUniformInt( const int loc, const int val );
	bool	SetUniformUInt( const int loc, const unsigned int val );
	bool	SetUniformFloat( const int loc, const float val );
	bool	SetUniformPtr( const int loc, const void * const ptr, const uniformType_e type, const int count = 1 );
private:
	bool		linked;
	int			program;

	int	*		shaders;
	int			shadersLen;
	static int	currentProgram;
};

#endif