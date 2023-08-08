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

#ifndef TEXTUREVERTSHADER_H
#define TEXTUREVERTSHADER_H

const char *getTexturedVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return "#version 330 core\n"
           "layout (location = 0) in vec3 aPos;"
           "layout (location = 1) in vec3 aColor;"
           "layout (location = 2) in vec2 aUV;"
           ""
           "uniform mat4 worldMatrix;" // World Model
           "uniform mat4 viewMatrix = mat4(1.0);" // default value for view matrix (identity)
           "uniform mat4 projectionMatrix = mat4(1.0);" 
           ""
           "out vec3 vertexColor;"
           "out vec2 vertexUV;"
           ""
           "void main()"
           "{"
           "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
           "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
           "   vertexColor = aColor;"
           "   vertexUV = aUV;"
           "}";
}

#endif /* TEXTUREVERTSHADER_H */
