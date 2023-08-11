/**
 * @file comp371.h
 *
 * CONCORDIA UNIVERSITY
 * COMP 371: COMPUTER GRAPHICS
 * Section CC
 * Professor: Kaustubha Mendhurwar
 *
 * TEAM PROJECT
 * DUE DATE: FRIDAY, AUGUST 11, 2023
 *
 * @author Joy Anne Valdez
 *
 * @date 2023-07-21
 *
 */

#ifndef COMP371_A2_H
#define COMP371_A2_H

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#define GLEW_STATIC 1
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vertexData.h"

#include "vertexShader.h"
#include "fragmentShader.h"

#include "textureVertShader.h"
#include "textureFragShader.h"

using namespace glm;
using namespace std;

// ATTRIBUTES / METHOTDS
GLuint loadTexture(const char *filename);      // LOAD Texture Files
const char *getVertexShaderSource();           // VERTEX SHADERS
const char *getFragmentShaderSource();         // FRAGMENT SHADERS
const char *getTexturedVertexShaderSource();   // TEXTURE VERTEX SHADERS
const char *getTexturedFragmentShaderSource(); // TEXTURE FRAGMENT SHADERS

// Model - View - Projection
void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix);
void setViewMatrix(int shaderProgram, mat4 viewMatrix);
void setWorldMatrix(int shaderProgram, mat4 worldMatrix);


int compileAndLinkShaders(const char *vertexShaderSource, const char *fragmentShaderSource);
int createTexturedCubeVertexArrayObject();

// Creating Vertex Array Objects VAOs ----------------------
int createVertexArrayObject(const glm::vec3 *vertexArray, int arraySize);

// Creating Vertex Buffer Objects VBOs ---------------------
int createVertexBufferObject();

int loadSHADER(string vertex_file_path, string fragment_file_path);
//*************************************************************************************************************
//*************************************************************************************************************

// Model - View - Projection ------------------------------------
void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
  glUseProgram(shaderProgram);
  GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
  glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
  glUseProgram(shaderProgram);
  GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
  glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
  glUseProgram(shaderProgram);
  GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
  glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}
//*************************************************************************************************************
// Linking of VERTEX / FRAGMENT / TEXTURE SHADERS --------------------------------------------------------
int compileAndLinkShaders(const char *vertexShaderSource, const char *fragmentShaderSource)
{
  // compile and link shader program
  // return shader program id
  // ------------------------------------

  // vertex shader
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // fragment shader
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // link shaders
  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}
// Load Textures --------------------------
GLuint loadTexture(const char *filename)
{
  // Step1 Create and bind textures
  GLuint textureId = 0;
  glGenTextures(1, &textureId);
  assert(textureId != 0);

  glBindTexture(GL_TEXTURE_2D, textureId);

  // Step2 Set filter parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Step3 Load Textures with dimension data
  int width, height, nrChannels;
  unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
  if (!data)
  {
    std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
    return 0;
  }

  // Step4 Upload the texture to the PU
  GLenum format = 0;
  if (nrChannels == 1)
    format = GL_RED;
  else if (nrChannels == 3)
    format = GL_RGB;
  else if (nrChannels == 4)
    format = GL_RGBA;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
               0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Step5 Free resources
  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);

  return textureId;
}
// VAO ------------------------------------------------------------------
int createVertexArrayObject(const glm::vec3 *vertexArray, int arraySize)
{
  // Create a vertex array
  GLuint vertexArrayObject;
  glGenVertexArrays(1, &vertexArrayObject);
  glBindVertexArray(vertexArrayObject);

  // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
  GLuint vertexBufferObject;
  glGenBuffers(1, &vertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);

  glVertexAttribPointer(0,                     // attribute 0 matches aPos in Vertex Shader
                        3,                     // size
                        GL_FLOAT,              // type
                        GL_FALSE,              // normalized?
                        2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
                        (void *)0              // array buffer offset
  );
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, // attribute 1 matches aColor in Vertex Shader
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        2 * sizeof(glm::vec3),
                        (void *)sizeof(glm::vec3) // color is offseted a vec3 (comes after position)
  );
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return vertexArrayObject;
}
//*************************************************************************************************************
// VBO ---------------------------------------------------------------
int createVertexBufferObject()
{
  // YELLOW Cube model
  vec3 yellowCube[] = {                                      // position,                            color
                       vec3(-0.5f, -0.5f, -0.5f), colors[0], // left
                       vec3(-0.5f, -0.5f, 0.5f), colors[0],
                       vec3(-0.5f, 0.5f, 0.5f), colors[0],

                       vec3(-0.5f, -0.5f, -0.5f), colors[0],
                       vec3(-0.5f, 0.5f, 0.5f), colors[0],
                       vec3(-0.5f, 0.5f, -0.5f), colors[0],

                       vec3(0.5f, 0.5f, -0.5f), colors[0], // far
                       vec3(-0.5f, -0.5f, -0.5f), colors[0],
                       vec3(-0.5f, 0.5f, -0.5f), colors[0],

                       vec3(0.5f, 0.5f, -0.5f), colors[0],
                       vec3(0.5f, -0.5f, -0.5f), colors[0],
                       vec3(-0.5f, -0.5f, -0.5f), colors[0],

                       vec3(0.5f, -0.5f, 0.5f), colors[0], // bottom
                       vec3(-0.5f, -0.5f, -0.5f), colors[0],
                       vec3(0.5f, -0.5f, -0.5f), colors[0],

                       vec3(0.5f, -0.5f, 0.5f), colors[0],
                       vec3(-0.5f, -0.5f, 0.5f), colors[0],
                       vec3(-0.5f, -0.5f, -0.5f), colors[0],

                       vec3(-0.5f, 0.5f, 0.5f), colors[0], // near
                       vec3(-0.5f, -0.5f, 0.5f), colors[0],
                       vec3(0.5f, -0.5f, 0.5f), colors[0],

                       vec3(0.5f, 0.5f, 0.5f), colors[0],
                       vec3(-0.5f, 0.5f, 0.5f), colors[0],
                       vec3(0.5f, -0.5f, 0.5f), colors[0],

                       vec3(0.5f, 0.5f, 0.5f), colors[0], // right
                       vec3(0.5f, -0.5f, -0.5f), colors[0],
                       vec3(0.5f, 0.5f, -0.5f), colors[0],

                       vec3(0.5f, -0.5f, -0.5f), colors[0],
                       vec3(0.5f, 0.5f, 0.5f), colors[0],
                       vec3(0.5f, -0.5f, 0.5f), colors[0],

                       vec3(0.5f, 0.5f, 0.5f), colors[0], // top
                       vec3(0.5f, 0.5f, -0.5f), colors[0],
                       vec3(-0.5f, 0.5f, -0.5f), colors[0],

                       vec3(0.5f, 0.5f, 0.5f), colors[0],
                       vec3(-0.5f, 0.5f, -0.5f), colors[0],
                       vec3(-0.5f, 0.5f, 0.5f), colors[0]};

  // Create a vertex array
  GLuint vertexArrayObject;
  glGenVertexArrays(1, &vertexArrayObject);
  glBindVertexArray(vertexArrayObject);

  // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
  GLuint vertexBufferObject;
  glGenBuffers(1, &vertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(yellowCube), yellowCube, GL_STATIC_DRAW);

  glVertexAttribPointer(0,                     // attribute 0 matches aPos in Vertex Shader
                        3,                     // size
                        GL_FLOAT,              // type
                        GL_FALSE,              // normalized?
                        2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
                        (void *)0              // array buffer offset
  );
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, // attribute 1 matches aColor in Vertex Shader
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        2 * sizeof(glm::vec3),
                        (void *)sizeof(glm::vec3) // color is offseted a vec3 (comes after position)
  );
  glEnableVertexAttribArray(1);

  return vertexBufferObject;
}
int createTexturedCubeVertexArrayObject()
{
  // Create a vertex array
  GLuint vertexArrayObject;
  glGenVertexArrays(1, &vertexArrayObject);
  glBindVertexArray(vertexArrayObject);

  // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
  GLuint vertexBufferObject;
  glGenBuffers(1, &vertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

  glVertexAttribPointer(0,                             // attribute 0 matches aPos in Vertex Shader
                        3,                             // size
                        GL_FLOAT,                      // type
                        GL_FALSE,                      // normalized?
                        sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
                        (void *)0                      // array buffer offset
  );
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, // attribute 1 matches aColor in Vertex Shader
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(TexturedColoredVertex),
                        (void *)sizeof(vec3) // color is offseted a vec3 (comes after position)
  );
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, // attribute 2 matches aUV in Vertex Shader
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(TexturedColoredVertex),
                        (void *)(2 * sizeof(vec3)) // uv is offseted by 2 vec3 (comes after position and color)
  );
  glEnableVertexAttribArray(2);

  return vertexArrayObject;
}
int loadSHADER(string vertex_file_path, string fragment_file_path)
{

  // Create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
  if (VertexShaderStream.is_open())
  {
    std::stringstream sstr;
    sstr << VertexShaderStream.rdbuf();
    VertexShaderCode = sstr.str();
    VertexShaderStream.close();
  }
  else
  {
    printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path.c_str());
    getchar();
    return 0;
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
  if (FragmentShaderStream.is_open())
  {
    std::stringstream sstr;
    sstr << FragmentShaderStream.rdbuf();
    FragmentShaderCode = sstr.str();
    FragmentShaderStream.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Compile Vertex Shader
  cout << "Compiling shader : " << vertex_file_path << endl;
  char const *VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0)
  {
    std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    printf("%s\n", &VertexShaderErrorMessage[0]);
  }

  // Compile Fragment Shader
  cout << "Compiling shader : " << fragment_file_path << endl;
  char const *FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0)
  {
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    printf("%s\n", &FragmentShaderErrorMessage[0]);
  }

  // Link the program
  printf("Linking program\n");
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0)
  {
    std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    printf("%s\n", &ProgramErrorMessage[0]);
  }

  glDetachShader(ProgramID, VertexShaderID);
  glDetachShader(ProgramID, FragmentShaderID);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;
}

#endif /* COMP371_A2_H */
