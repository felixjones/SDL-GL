#version 330

layout( location = 0 ) in vec3 glVertex_position;
layout( location = 1 ) in vec3 glVertex_normal;
layout( location = 2 ) in vec4 glVertex_colour;
layout( location = 3 ) in vec2 glVertex_uv;

out vec4 glFragment_colour;
out vec2 glFragment_uv;
out vec3 glFragment_normal;
out vec4 glFragment_position;

uniform mat4	uniform_matrix;
  
void main() {
    vec4 vertexPosition = vec4( glVertex_position, 1.0 );
	gl_Position = uniform_matrix * vertexPosition;
	
	vec3 vertexNormal = ( uniform_matrix * vec4( glVertex_normal, 0.0 ) ).xyz;
	
    glFragment_uv = glVertex_uv;
	glFragment_colour.rgb = dot( vec3( 0.0, 0.0, -1.0 ), vertexNormal ); //glVertex_colour;
	glFragment_colour.a = 1.0;
	
	glFragment_position = gl_Position;
}