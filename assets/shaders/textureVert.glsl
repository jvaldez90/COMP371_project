#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 world_matrix;
uniform mat4 view_matrix = mat4(1.0);
uniform mat4 projection_matrix = mat4(1.0);
uniform mat4 light_view_proj_matrix = mat4(1.0);


out vec2 vertexUV;
out vec3 fragment_normal;
out vec3 fragment_position;
out vec4 fragment_position_light_space;


void main()
{
	fragment_normal = mat3(world_matrix) * aNormal;
	fragment_position = vec3(world_matrix * vec4(aPos, 1.0));
	fragment_position_light_space = light_view_proj_matrix * vec4(fragment_position, 1.0);


	mat4 modelViewProjection = projection_matrix * view_matrix * world_matrix;
	gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vertexUV = aUV;
}