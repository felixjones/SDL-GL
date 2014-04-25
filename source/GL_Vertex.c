#include "GL_Vertex.h"

#define GLEW_STATIC
#include <GL/glew.h>

/*
typedef struct xiGLVertex_s {
	vec3_t	position;
	vec3_t	normal;
	vec4_t	colour;
	vec2_t	uv;
} xiGLVertex_t;
*/
static xiGLVertex_t	xiGLVertex;

void GLVertex_SetupAttributes() {
	glVertexAttribPointer( 0, VEC3, GL_FLOAT, GL_FALSE, sizeof( xiGLVertex ), 0 ); // POSITION
	glVertexAttribPointer( 1, VEC3, GL_FLOAT, GL_FALSE, sizeof( xiGLVertex ), ( void * )( sizeof( xiGLVertex.position ) ) ); // NORMAL
	glVertexAttribPointer( 2, VEC4, GL_FLOAT, GL_FALSE, sizeof( xiGLVertex ), ( void * )( sizeof( xiGLVertex.position ) + sizeof( xiGLVertex.normal ) ) ); // COLOUR
	glVertexAttribPointer( 3, VEC2, GL_FLOAT, GL_FALSE, sizeof( xiGLVertex ), ( void * )( sizeof( xiGLVertex.position ) + sizeof( xiGLVertex.normal ) + sizeof( xiGLVertex.colour ) ) ); // UV
}