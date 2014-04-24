#version 330

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 vertexUV;

out vec2 fragUV;
  
void main() {
    gl_Position = vec4( vertexPos, 1.0 );
 
    fragUV = vertexUV;
}