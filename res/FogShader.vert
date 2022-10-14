#version 400 //TODO: rename?

layout (location = 0) in vec3 VertexPosition;

uniform mat4 transform;

out vec4 v_pos; 

uniform mat4 ModelMatrix;

void main()
{
	v_pos = ModelMatrix * vec4(VertexPosition, 1.0);
	gl_Position = transform * vec4(VertexPosition, 1.0);
}
