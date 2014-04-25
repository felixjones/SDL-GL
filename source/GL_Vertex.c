#include "GL_Vertex.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define OFFSET_OF( X, Y )	( size_t )&( ( ( X * )0 )->Y )

void GLVertex_SetupAttributes() {
	glVertexAttribPointer( 0, VEC3, GL_FLOAT, GL_FALSE, sizeof( xiGLVertex_t ), ( void * )( OFFSET_OF( xiGLVertex_t, position ) ) ); // POSITION
	glVertexAttribPointer( 1, VEC3, GL_FLOAT, GL_FALSE, sizeof( xiGLVertex_t ), ( void * )( OFFSET_OF( xiGLVertex_t, normal ) ) ); // NORMAL
	glVertexAttribPointer( 2, VEC4, GL_FLOAT, GL_FALSE, sizeof( xiGLVertex_t ), ( void * )( OFFSET_OF( xiGLVertex_t, colour ) ) ); // COLOUR
	glVertexAttribPointer( 3, VEC2, GL_FLOAT, GL_FALSE, sizeof( xiGLVertex_t ), ( void * )( OFFSET_OF( xiGLVertex_t, uv ) ) ); // UV
}