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

#ifndef FRAGMENTSHADER_H
#define FRAGMENTSHADER_H

const char *getFragmentShaderSource()
{
   return "#version 330 core\n"
          "out vec4 FragColor;"
          ""
          "in vec3 vertexColor;"
          ""
          "void main()"
          "{"

          "   FragColor = vec4(vertexColor, 1.0f);"
          "}";
}

#endif /* FRAGMENTSHADER_H */
