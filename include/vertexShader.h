#include <iostream>
#include <list>

#define GLEW_STATIC 1
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

using namespace glm;
using namespace std;

#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

const char *getVertexShaderSource()
{
    return "#version 330 core\n"
           "layout (location = 0) in vec3 aPos;"
           "layout (location = 1) in vec3 aColor;"
           ""
           "uniform mat4 worldMatrix;"
           "uniform mat4 viewMatrix = mat4(1.0);" // default value for view matrix (identity)
           "uniform mat4 projectionMatrix = mat4(1.0);"
           ""
           "out vec3 vertexColor;"
           "void main()"
           "{"
           "   vertexColor = aColor;"
           "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
           "   gl_Position = modelViewProjection * vec4(aPos, 1.0);"
           "}";
}

#endif /* VERTEXSHADER_H */
