#ifndef LIGHTVERT_H
#define LIGHTVERT_H

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

const char *getLightVertexShaderSource()
{
	return "#version 330 core"
		   ""
		   "layout (location = 0) in vec3 aPos;"
		   ""
		   "uniform mat4 worldMatrix;"
		   "uniform mat4 viewMatrix;"

		   "void main()"
		   "{"
		   "	gl_Position = camMatrix * model * vec4(aPos, 1.0f);"
		   "}";
}
#endif /* LIGHTVERT_H */
// Reference: OpenGL Tutorial 9 : Lighting
// https://www.youtube.com/watch?v=ZbszezwNSZU&list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-&index=10