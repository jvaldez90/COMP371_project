#ifndef MODELOBJECTS_H
#define MODELOBJECTS_H

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

// #include "comp371.h"
#include "vertexData.h"

using namespace glm;
using namespace std;

// For Changing the rendering of the geometric shapes
GLenum renderMode = GL_TRIANGLES;

// OBJECT CREATIONS ---------------------------------------
void draw_J_racket(GLuint shaderProgram, vec3 translateBy, float degrees, vec3 rotateOn, vec3 scaleBy, GLenum renderMode);
void draw_arm(GLuint shaderProgram, vec3 translateBy, float degrees, vec3 rotateOn, vec3 scaleBy, GLenum renderMode);

void draw_crowd(GLuint shaderProgram, vec3 translateBy, float degrees, vec3 rotateOn);
void draw_scoreboard(GLuint shaderProgram, vec3 translateBy, vec3 scaleBy, float degrees, vec3 rotateOn);

/********************************************************************************************************/
/*************************
 * DRAWING MODEL OBJECTS *
 *************************/
// Drawing the Tennis Racket -----------------------------------------
void draw_J_racket(GLuint shaderProgram, vec3 translateBy, float degrees, vec3 rotateOn, vec3 scaleBy, GLenum renderMode)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);

    // Create a scalar vector as base reference
    vec3 scalar = vec3(0.2f, 0.2f, 0.2f); // Re-scales pre-defined cube size

    // Center Net of Racket -----------------------------------------------------------
    SetUniformVec3(shaderProgram, "object_color", colors[3]); // Set active color to Green
    // using a for-loop for grid pattern
    mat4 racketNet = mat4(1.0);

    vec3 RACKET_NET_SCALED = vec3(0.02f, 0.45f, 0.02f);
    float RACKET_NET_WIDTH = 0.8f;   // Adjustments in relevance to world model orientation
    float RACKET_NET_HEIGHT = 5.45f; // Adjustments in relevance to world model orientation

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 11; ++j)
        {
            // Horizontal lines
            racketNet = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateOn) * scale(mat4(1.0), scaleBy) *
                        // Line above is for Adjustments in relevance to world model orientation
                        translate(mat4(1.0), vec3(RACKET_NET_WIDTH + i * 0.2f, RACKET_NET_HEIGHT + j * 0.2, 0.0f)) *
                        rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), RACKET_NET_SCALED);
            SetUniformMat4(shaderProgram, "world_matrix", racketNet);
            glDrawArrays(renderMode, 0, 36);

            // Vertcal lines
            racketNet = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateOn) * scale(mat4(1.0), scaleBy) *
                        // Line above is for Adjustments in relevance to world model orientation
                        translate(mat4(1.0), vec3(RACKET_NET_WIDTH + i * 0.2f, RACKET_NET_HEIGHT + j * 0.2f, 0.0f)) *
                        scale(mat4(1.0), RACKET_NET_SCALED);
            SetUniformMat4(shaderProgram, "world_matrix", racketNet);
            glDrawArrays(renderMode, 0, 36);
        }
    }

    // Upper racket --------------------------------------------------
    SetUniformVec3(shaderProgram, "object_color", colors[7]); // Set active color to Gray

    vec3 TOP_SIDES_SCALED = vec3(0.17f, 0.85f, 0.17f);
    float TOP_SIDES_HEIGHT = 7.625f;

    // left top
    mat4 leftTop = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateOn) * scale(mat4(1.0), scaleBy) *
                   // Line above is for Adjustments in relevance to world model orientation
                   scale(mat4(1.0), scalar) *
                   translate(mat4(1.0), vec3(0.95f, TOP_SIDES_HEIGHT, 0.0f)) *
                   rotate(mat4(1.0), radians(-65.0f), vec3(0.0f, 0.0f, 1.0f)) *
                   scale(mat4(1.0), TOP_SIDES_SCALED);
    SetUniformMat4(shaderProgram, "world_matrix", leftTop);
    glDrawArrays(renderMode, 0, 36);

    // right top
    mat4 rightTop = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateOn) * scale(mat4(1.0), scaleBy) *
                    // Line above is for Adjustments in relevance to world model orientation
                    scale(mat4(1.0), scalar) *
                    translate(mat4(1.0), vec3(2.05f, TOP_SIDES_HEIGHT, 0.0f)) *
                    rotate(mat4(1.0), radians(65.0f), vec3(0.0f, 0.0f, 1.0f)) *
                    scale(mat4(1.0), TOP_SIDES_SCALED);
    SetUniformMat4(shaderProgram, "world_matrix", rightTop);
    glDrawArrays(renderMode, 0, 36);

    // middle top
    SetUniformVec3(shaderProgram, "object_color", colors[6]); // Set active color Red

    mat4 middleTop = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateOn) * scale(mat4(1.0), scaleBy) *
                     // Line above is for Adjustments in relevance to world model orientation
                     scale(mat4(1.0), scalar) *
                     translate(mat4(1.0), vec3(1.5f, 7.8f, 0.0f)) *
                     rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                     scale(mat4(1.0), vec3(0.17f, 0.4f, 0.17f));
    SetUniformMat4(shaderProgram, "world_matrix", middleTop);
    glDrawArrays(renderMode, 0, 36);

    // Sides of Racket ---------------------------------------------------
    vec3 SIDES_SCALED = vec3(0.17f, 2.0f, 0.17f);
    float SIDES_HEIGHT = 6.5f;

    // left side
    mat4 leftSide = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateOn) * scale(mat4(1.0), scaleBy) *
                    // Line above is for Adjustments in relevance to world model orientation
                    scale(mat4(1.0), scalar) *
                    translate(mat4(1.0), vec3(0.6f, SIDES_HEIGHT, 0.0f)) *
                    scale(mat4(1.0), SIDES_SCALED);
    SetUniformMat4(shaderProgram, "world_matrix", leftSide);
    glDrawArrays(renderMode, 0, 36);

    // right side
    mat4 rightSide = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateOn) * scale(mat4(1.0), scaleBy) *
                     // Line above is for Adjustments in relevance to world model orientation
                     scale(mat4(1.0), scalar) *
                     translate(mat4(1.0), vec3(2.4f, SIDES_HEIGHT, 0.0f)) *
                     scale(mat4(1.0), SIDES_SCALED);
    SetUniformMat4(shaderProgram, "world_matrix", rightSide);
    glDrawArrays(renderMode, 0, 36);

    // Base of Racket ------------------------------------------------------
    vec3 BASE_SIDES_SCALED = vec3(0.17f, 1.25f, 0.17f);
    float BASE_SIDES_HEIGHT = 5.1f;

    // middle base
    mat4 middleBase = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateOn) * scale(mat4(1.0), scaleBy) *
                      // Line above is for Adjustments in relevance to world model orientation
                      scale(mat4(1.0), scalar) * translate(mat4(1.0), vec3(1.5f, 5.15f, 0.0f)) *
                      rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                      scale(mat4(1.0), vec3(0.15f, 1.1f, 0.15f));
    SetUniformMat4(shaderProgram, "world_matrix", middleBase);
    glDrawArrays(renderMode, 0, 36);

    // left base
    SetUniformVec3(shaderProgram, "object_color", colors[7]); // Set active color Gray

    mat4 leftBase = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateOn) * scale(mat4(1.0), scaleBy) *
                    // Line above is for Adjustments in relevance to world model orientation
                    scale(mat4(1.0), scalar) * translate(mat4(1.0), vec3(1.05f, BASE_SIDES_HEIGHT, 0.0f)) *
                    rotate(mat4(1.0), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                    scale(mat4(1.0), BASE_SIDES_SCALED);
    SetUniformMat4(shaderProgram, "world_matrix", leftBase);
    glDrawArrays(renderMode, 0, 36);

    // right base
    mat4 rightBase = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateOn) * scale(mat4(1.0), scaleBy) *
                     // Line above is for Adjustments in relevance to world model orientation
                     scale(mat4(1.0), scalar) * translate(mat4(1.0), vec3(1.95f, BASE_SIDES_HEIGHT, 0.0f)) *
                     rotate(mat4(1.0), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                     scale(mat4(1.0), BASE_SIDES_SCALED);
    SetUniformMat4(shaderProgram, "world_matrix", rightBase);
    glDrawArrays(renderMode, 0, 36);

    // Racket Handle --------------------------------------------------
    SetUniformVec3(shaderProgram, "object_color", colors[6]); // Set active color Red

    mat4 handleRacket = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateOn) * scale(mat4(1.0), scaleBy) *
                        // Line above is for Adjustments in relevance to world model orientation
                        scale(mat4(1.0), scalar) *
                        translate(mat4(1.0), vec3(1.5f, 3.5f, 0.0f)) *
                        rotate(mat4(1.0), radians(0.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), vec3(0.15f, 2.5f, 0.15f));
    SetUniformMat4(shaderProgram, "world_matrix", handleRacket);
    glDrawArrays(renderMode, 0, 36);

    draw_arm(shaderProgram, translateBy, degrees, rotateOn, scaleBy, renderMode);
}
//*************************************************************************************************************
// DRAWING the ARM ---------------------------------------------
void draw_arm(GLuint shaderProgram, vec3 translateBy, float degrees, vec3 rotateOn, vec3 scaleBy, GLenum renderMode)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);

    // Create a scalar vector as base reference
    vec3 scalar = vec3(0.2f, 0.2f, 0.2f);

    SetUniformVec3(shaderProgram, "object_color", colors[8]); // Set active color Beige for Arm
    vec3 ARM_SCALED = vec3(0.25f, 1.5f, 0.25f);

    // Hand
    mat4 hand = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateOn) * scale(mat4(1.0), scaleBy) *
                // Line above is for Adjustments in relevance to world model orientation
                scale(mat4(1.0), scalar) *
                translate(mat4(1.0), vec3(1.5f, 3.0f, 0.0f)) *
                scale(mat4(1.0), vec3(0.25f, 0.25f, 0.25f));
    SetUniformMat4(shaderProgram, "world_matrix", hand);
    glDrawArrays(renderMode, 0, 36);

    // Forearm
    mat4 forearm = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateOn) * scale(mat4(1.0), scaleBy) *
                   // Line above is for Adjustments in relevance to world model orientation
                   scale(mat4(1.0), scalar) *
                   translate(mat4(1.0), vec3(1.5f, 3.0f, 0.0f)) *
                   scale(mat4(1.0), vec3(0.25f, 0.25f, 0.25f));
    SetUniformMat4(shaderProgram, "world_matrix", forearm);
    glDrawArrays(renderMode, 0, 36);

    // Bicep
    mat4 bicep = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateOn) * scale(mat4(1.0), scaleBy) *
                 // Line above is for Adjustments in relevance to world model orientation
                 scale(mat4(1.0), scalar) *
                 translate(mat4(1.0), vec3(-0.6f, 3.53f, 0.0f)) *
                 rotate(mat4(1.0), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                 scale(mat4(1.0), vec3(1.5f, 0.25f, 0.25f));
    SetUniformMat4(shaderProgram, "world_matrix", bicep);
    glDrawArrays(renderMode, 0, 36);
}
//*************************************************************************************************************
// Set up for a crowd into environment
void draw_crowd(GLuint shaderProgram, vec3 translateBy, float degrees, vec3 rotateOn)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);

    SetUniformVec3(shaderProgram, "object_color", colors[1]); // Set active color White

    mat4 crowd = translate(mat4(1.0), translateBy) *
                 rotate(mat4(1.0), radians(degrees), rotateOn) *               // Rotation placement in world model
                 rotate(mat4(1.0), radians(-240.0f), vec3(1.0f, 0.0f, 0.0f)) * // Adjusts texture orientation
                 scale(mat4(1.0), vec3(8.0, 8.0, 0.05f));
    SetUniformMat4(shaderProgram, "world_matrix", crowd);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
//*************************************************************************************************************
// Drawing a scoreboard
void draw_scoreboard(GLuint shaderProgram, vec3 translateBy, vec3 scaleBy, float degrees, vec3 rotateOn)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);

    SetUniformVec3(shaderProgram, "object_color", colors[9]); // Set active color Dark Green

    mat4 scoreboard = translate(mat4(1.0), translateBy) *
                      rotate(mat4(1.0), radians(degrees), rotateOn) *              // Rotation placement in world model
                      rotate(mat4(1.0), radians(180.0f), vec3(1.0f, 0.0f, 0.0f)) * // Adjusts texture orientation
                      scale(mat4(1.0), scaleBy);
    SetUniformMat4(shaderProgram, "world_matrix", scoreboard);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

#endif /* MODELOBJECTS_H */
