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

#ifndef TEXTUREFRAGSHADER_H
#define TEXTUREFRAGSHADER_H

const char *getTexturedFragmentShaderSource()
{
    return "#version 330 core\n"
           "in vec3 vertexColor;"
           "in vec2 vertexUV;"
           ""
           "uniform sampler2D textureSampler;"
           "uniform sampler2D textureSampler2;"
           "uniform sampler2D textureSampler3;"
           "uniform sampler2D textureSampler4;"
           ""
           "out vec4 FragColor;"
           "void main()"
           "{"
           "   vec4 textureColor = texture( textureSampler, vertexUV );"
           "   vec4 textureColor2 = texture( textureSampler2, vertexUV );"
           "   vec4 textureColor3 = texture( textureSampler3, vertexUV );"
           "   vec4 textureColor4 = texture( textureSampler4, vertexUV );"
           "   FragColor = textureColor4;"
           "}";
}

#endif /* TEXTUREFRAGSHADER_H */
