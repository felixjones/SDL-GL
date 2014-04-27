#version 330

layout( location = 0 ) in vec3 glVertex_position;
layout( location = 1 ) in vec3 glVertex_normal;
layout( location = 2 ) in vec4 glVertex_colour;
layout( location = 3 ) in vec2 glVertex_uv;

out vec4 glFragment_colour;
out vec2 glFragment_uv;
  
void main() {
    gl_Position = vec4( glVertex_position, 1.0 );
	
    glFragment_uv = glVertex_uv;
	glFragment_colour = glVertex_colour;
}