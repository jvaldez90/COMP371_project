#include <iostream>

#include <vector>


#define GLEW_STATIC 1                     // this allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>                      // include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h>                   // GLFW provides a cross-platform interface for creating a graphical context,
                                          // initializing OpenGL and binding inputs

#include <glm/glm.hpp>                    // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp>   // include this to create transformation matrices
#include <glm/gtc/type_ptr.hpp>           // this allows to use value_ptr which is used for specifying the appropriet color of each model part

using namespace glm;
using namespace std;


// Function declaration:
void SetUniformMat4(GLuint shader_id, const char* uniform_name, mat4 uniform_value);
void SetUniformVec3(GLuint shader_id, const char* uniform_name, vec3 uniform_value);
//template <class T>;
//void SetUniform1Value(GLuint shader_id, const char* uniform_name, T uniform_value);

//void drawLight(GLuint shaderProgram, vec3 scaleBy, vec3 translateBy);


// Draw the light object:
void drawLight(GLuint shaderProgram, vec3 scaleBy, vec3 translateBy) {
    
    // Use proper shader:
    glUseProgram(shaderProgram);
    
    mat4 lightObj = mat4(1.0f);                                 // matrix corresponding to the light object
    
    lightObj = translate(mat4(1.0f), translateBy) * scale(mat4(1.0f), scaleBy);

    SetUniformMat4(shaderProgram, "world_matrix", lightObj);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}


// Draw a world ground:
void drawGround(GLuint shaderProgram, vec3 scaleBy, vec3 translateBy) {

    // Use proper shader:
    glUseProgram(shaderProgram);

    vec3 worldTranslation = vec3(1.0f, 1.0f, 1.0f);
    mat4 groundMatrix = mat4(1.0f);                             // matrix corresponding to the ground surface

    groundMatrix = translate(mat4(1.0), translateBy) * scale(mat4(1.0), scaleBy);

    SetUniformMat4(shaderProgram, "world_matrix", groundMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawCourt(GLuint shaderProgramCourt, GLuint shaderProgramLines, vec3 scaleBy, vec3 translateBy) {

    // Use proper shader:
    glUseProgram(shaderProgramCourt);

    mat4 courtWorldMatrix = mat4(1.0f);                         // matrix corresponding to the court surface
    mat4 lineMatrix = mat4(1.0f);                               // matrix corresponding to the court surface lines

    vec3 scaled_L = vec3(1.0f, 1.0f, 1.0f);                                                                         // length scaled (4)
    vec3 scaled_W = vec3(1.0f, 1.0f, 1.0f);                                                                         // width scaled (2)
    vec3 scaled_S = vec3(1.0f, 1.0f, 1.0f);                                                                         // service line scaled (2)
    vec3 scaled_C = vec3(1.0f, 1.0f, 1.0f);                                                                         // center mark scaled (2)

    courtWorldMatrix = translate(mat4(1.0), translateBy) * scale(mat4(1.0), scaleBy);

    SetUniformMat4(shaderProgramCourt, "world_matrix", courtWorldMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw court lines:

    // Use proper shader:
    glUseProgram(shaderProgramLines);

    scaled_L = vec3((scaleBy[0] + 1), scaleBy[1], 1.0f);                                                            
    scaled_W = vec3(1.0f, scaleBy[1], scaleBy[2]);                                                                  
    scaled_S = vec3(1.0f, scaleBy[1], (scaleBy[2] / 1.33f));                                                        
    scaled_C = vec3((scaleBy[0] / 30.6), scaleBy[1], 1.0f);                                                         

    SetUniformVec3(shaderProgramCourt, "object_color", vec3(1.0f, 1.0f, 1.0f));                                     // updates the color of the court lines

    // Lines drawn along the length side:
    lineMatrix = translate(mat4(1.0), vec3(0.0f, 0.0f, -(scaleBy[2] / 2))) * scale(mat4(1.0), scaled_L);            // outer line in negative z-axis
    SetUniformMat4(shaderProgramLines, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lineMatrix = translate(mat4(1.0), vec3(0.0f, 0.0f, (-(scaleBy[2] / 2) / 1.33f))) * scale(mat4(1.0), scaled_L);  // inner line in negative z-axis
    SetUniformMat4(shaderProgramLines, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lineMatrix = translate(mat4(1.0), vec3(0.0f, 0.0f, ((scaleBy[2] / 2) / 1.33f))) * scale(mat4(1.0), scaled_L);   // inner line in positive z-axis
    SetUniformMat4(shaderProgramLines, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lineMatrix = translate(mat4(1.0), vec3(0.0f, 0.0f, (scaleBy[2] / 2))) * scale(mat4(1.0), scaled_L);             // outer line in positive z-axis
    SetUniformMat4(shaderProgramLines, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Lines drawn along the width side:
    lineMatrix = translate(mat4(1.0), vec3(-(scaleBy[0] / 2.0f), 0.0f, 0.0f)) * scale(mat4(1.0), scaled_W);
    SetUniformMat4(shaderProgramLines, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lineMatrix = translate(mat4(1.0), vec3((scaleBy[0] / 2.0f), 0.0f, 0.0f)) * scale(mat4(1.0), scaled_W);
    SetUniformMat4(shaderProgramLines, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Service lines:
    lineMatrix = translate(mat4(1.0), vec3(-(scaleBy[0] / 3.74f), 0.0f, 0.0f)) * scale(mat4(1.0), scaled_S);
    SetUniformMat4(shaderProgramLines, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lineMatrix = translate(mat4(1.0), vec3((scaleBy[0] / 3.74f), 0.0f, 0.0f)) * scale(mat4(1.0), scaled_S);
    SetUniformMat4(shaderProgramLines, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Center Mark lines:
    lineMatrix = translate(mat4(1.0), vec3(-(scaleBy[0] / 2.07f), 0.0f, 0.0f)) * scale(mat4(1.0), scaled_C);
    SetUniformMat4(shaderProgramLines, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lineMatrix = translate(mat4(1.0), vec3((scaleBy[0] / 2.07f), 0.0f, 0.0f)) * scale(mat4(1.0), scaled_C);
    SetUniformMat4(shaderProgramLines, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawPoles(GLuint shaderProgram, vec3 scaleBy, vec3 translateBy) {
    mat4 tennisPoleMatrix = mat4(1.0f);             // matrix corresponding to the tennis court poles

    tennisPoleMatrix = translate(mat4(1.0), translateBy) * scale(mat4(1.0), scaleBy);
    SetUniformMat4(shaderProgram, "world_matrix", tennisPoleMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    tennisPoleMatrix = translate(mat4(1.0), vec3(translateBy[0], translateBy[1], 0.0f)) * scale(mat4(1.0), scaleBy);
    SetUniformMat4(shaderProgram, "world_matrix", tennisPoleMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    tennisPoleMatrix = translate(mat4(1.0), vec3(translateBy[0], translateBy[1], -translateBy[2])) * scale(mat4(1.0), scaleBy);
    SetUniformMat4(shaderProgram, "world_matrix", tennisPoleMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawPoleNet(GLuint shaderProgram, vec3 scaleBy, vec3 translateBy) {
    mat4 tennisNetMatrix = mat4(1.0f);              // matrix corresponding to the tennis pole net

    // Declaration of floats used in the model drawing process:
    float netY = 0.0f;
    float netZ = 0.0f;

    // scaledNet = vec3(0.2f, 5.0f, 0.2f);
    netY = (scaleBy[1] / 2);
    netZ = translateBy[2];

    tennisNetMatrix = translate(mat4(1.0), translateBy) * scale(mat4(1.0), scaleBy);

    for (int i = 0; i < scaleBy[1]; ++i)
        for (int j = 0; j < -translateBy[2]; ++j) {
            
            // Horizontal lines along the z plane:
            tennisNetMatrix = translate(mat4(1.0), vec3(0.0f, netY + i * 2.0f, netZ + j * 2.0f))
                                * rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 1.0f, 0.0f))
                                * rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0), scaleBy);
            SetUniformMat4(shaderProgram, "world_matrix", tennisNetMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Horizontal lines along the x plane:
            tennisNetMatrix = translate(mat4(1.0), vec3(0.0f, netY + i * 2.0, netZ + j * 2.0f)) * scale(mat4(1.0), scaleBy);
            SetUniformMat4(shaderProgram, "world_matrix", tennisNetMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

    // Top of tennis net:
    tennisNetMatrix = translate(mat4(1.0), vec3(0.0f, 2.5 * scaleBy[1], 0.0f)) * scale(mat4(1.0), vec3(0.2, 0.5f, (2 * (-translateBy[2]))));
    SetUniformMat4(shaderProgram, "world_matrix", tennisNetMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Bottom of tennis net:
    tennisNetMatrix = translate(mat4(1.0), vec3(0.0f, 0.1f, 0.0f)) * scale(mat4(1.0), vec3(0.2, 0.5f, (2 * (-translateBy[2]))));
    SetUniformMat4(shaderProgram, "world_matrix", tennisNetMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
























