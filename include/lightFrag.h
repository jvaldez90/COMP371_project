#ifndef LIGHTFRAG_H
#define LIGHTFRAG_H

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

const char *getLightFragmentShaderSource()
{
    return "version 330 core"
           ""
           "out vec4 FragColor;"
           ""
           "uniform vec4 lightColor;"
           ""
           "void main()"
           "{"
           "	FragColor = lightColor;"
           "}";
}
#endif /* LIGHTFRAG_H */
// Reference: OpenGL Tutorial 9 : Lighting
// https://www.youtube.com/watch?v=ZbszezwNSZU&list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-&index=10