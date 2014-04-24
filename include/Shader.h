#ifndef __SHADER_H__
#define __SHADER_H__

#include <stdint.h>

class xiShader {
public:
	enum shaderType_e {
		SHADER_COMPUTE,
		SHADER_VERTEX,
		SHADER_TESS_CONTROL,
		SHADER_TESS_EVALUATION,
		SHADER_GEOMETRY,
		SHADER_FRAGMENT
	};

	static xiShader *	Get();
	void				Retain();
	void				Release();
	int					Compile( const char * const fileName, const shaderType_e type );
protected:
	xiShader();
	~xiShader();
private:
	typedef struct shader_s {
		char			name[80];
		int				glHandle;
		shaderType_e	type;
	} shader_t;

	static int	Shaders_Compare( const void * const left, const void * const right );
	void		Shaders_Sort();
	shader_t *	Shaders_Find( const char * const name );
	void		Shaders_PushBack( shader_t * const newShader );

	static xiShader *	singleton;
	static int32_t		references;

	struct shaders_s {
		shader_t *	ptr;
		int32_t		len;
	} shaders;
};

#endif