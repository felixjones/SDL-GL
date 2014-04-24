#version 330

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 vertexUV;

uniform float	shade;

out vec2 fragUV;
  
void main() {
    vec4 position = vec4( vertexPos, 1.0 );
	
	mat4 RotationMatrix = mat4( cos(shade), -sin(shade), 0.0, 0.0,
                               sin(shade),  cos(shade), 0.0, 0.0,
                               0.0,           0.0, 1.0, 0.0,
                               0.0,           0.0, 0.0, 1.0 );
    gl_Position = RotationMatrix * position;
	
    fragUV = vertexUV;
}