#ifndef __GL_VERTEX_H__
#define __GL_VERTEX_H__

#if defined( __cplusplus )
extern "C" {
#endif

#include "Vector.h"

typedef struct xiGLVertex_s {
	vec4_t	position;
	vec4_t	normal;
	vec4_t	colour;
	vec2_t	uv;
} xiGLVertex_t;

void	GLVertex_SetupAttributes();

#if defined( __cplusplus )
}
#endif

#endif