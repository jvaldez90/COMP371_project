#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 world_matrix;
uniform mat4 view_matrix = mat4(1.0);
uniform mat4 projection_matrix = mat4(1.0);

void main()
{
	mat4 modelViewProjection = projection_matrix * view_matrix * world_matrix;
	gl_Position = modelViewProjection * vec4(aPos, 1.0);
}