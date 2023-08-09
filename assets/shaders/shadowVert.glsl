#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 world_matrix;
uniform mat4 light_view_proj_matrix;


void main()
{
    mat4 scale_bias_matrix = mat4(vec4(0.5, 0.0, 0.0, 0.0),
                                    vec4(0.0, 0.5, 0.0, 0.0),
                                    vec4(0.0, 0.0, 0.5, 0.0),
                                    vec4(0.5, 0.5, 0.5, 1.0));

    gl_Position = scale_bias_matrix * light_view_proj_matrix * world_matrix
                    * vec4(position, 1.0);
}
