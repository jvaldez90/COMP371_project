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

#include "comp371.h"
#include "vertexData.h"

#include "vertexShader.h"
#include "fragmentShader.h"

#include "textureVertShader.h"
#include "textureFragShader.h"

using namespace glm;
using namespace std;

// For Changing the rendering of the geometric shapes
GLenum renderMode = GL_TRIANGLES;

// OBJECT CREATIONS ---------------------------------------
void draw_grid(int cubeAO, GLuint shaderProgram, vec3 translateBy);

void draw_net(GLuint shaderProgram, vec3 translateBy);
void draw_court(GLuint shaderProgram, vec3 translateBy);
void draw_courtLines(GLuint shaderProgram, vec3 translateBy);
void draw_ground(GLuint shaderProgram, vec3 translateBy);

void draw_V(GLuint shaderProgram, vec3 translateBy, float degrees, vec3 rotateBy, vec3 scaleBy);

void draw_racket(GLuint shaderProgram, vec3 translateBy, float degrees, vec3 rotateBy, vec3 scaleBy, GLenum renderMode);
void draw_arm(GLuint shaderProgram, vec3 translateBy, float degrees, vec3 rotateBy, vec3 scaleBy, GLenum renderMode);

void draw_crowd(GLuint shaderProgram, vec3 translateBy, float degrees, vec3 rotation);
void draw_scoreboard(GLuint shaderProgram, vec3 translateBy, vec3 scaleBy);

/********************************************************************************************************/
/*************************
 * DRAWING MODEL OBJECTS *
 *************************/
// DRAW GROUND GRID and AXIS ORIGIN ------------------------------------------------------------------
void draw_grid(int texturedCubeVAO, GLuint shaderProgram, vec3 translateBy)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);

    // Define and upload geometry to the GPU
    glBindVertexArray(texturedCubeVAO);

    vec3 GROUND_GRID_SCALED = vec3(0.01f, 0.5f, 0.01f);
    translateBy = vec3(-2.8f, 0.0f, 1.4f);
    mat4 groundWorldMatrix = mat4(1.0);

    for (int i = 0; i < 78; ++i) // Along the X axis: -4.8 -> 10.4,
    {
        for (int j = 0; j < 36; ++j) // Along the Z axis: -4.8 -> 2
        {
            // Horizontal lines along the z plane
            groundWorldMatrix = translate(mat4(1.0), translateBy) *
                                translate(mat4(1.0), vec3(-5.0f + i * 0.2f, 0.0f, -5.0f + j * 0.2f)) *
                                rotate(mat4(1.0), radians(90.0f), vec3(1.0f, 0.0f, 0.0f)) *
                                scale(mat4(1.0), GROUND_GRID_SCALED);
            SetUniformMat4(shaderProgram, "world_matrix", groundWorldMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram

            // Horizontal lines along the x plane
            groundWorldMatrix = translate(mat4(1.0), translateBy) *
                                translate(mat4(1.0), vec3(-5.0f + i * 0.2, 0.0f, -5.0f + j * 0.2f)) *
                                rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                                scale(mat4(1.0), GROUND_GRID_SCALED);
            SetUniformMat4(shaderProgram, "world_matrix", groundWorldMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram
        }
    }

    int redAO = createTexturedCubeVertexArrayObject(); // Red cube for X-axis
    glBufferData(GL_ARRAY_BUFFER, sizeof(redCube), redCube, GL_STATIC_DRAW);

    int greenAO = createTexturedCubeVertexArrayObject(); // Green cube for Y-axis
    glBufferData(GL_ARRAY_BUFFER, sizeof(greenCube), greenCube, GL_STATIC_DRAW);

    int blueAO = createTexturedCubeVertexArrayObject(); // Blue cube for Z-axis
    glBufferData(GL_ARRAY_BUFFER, sizeof(blueCube), blueCube, GL_STATIC_DRAW);

    // Drawing axis origin ------------------------------------------------
    // DRAW RED CUBE for X-Axis
    glBindVertexArray(redAO);

    mat4 xAxisMatrix = translate(mat4(1.0), vec3(0.5f, 0.0f, 0.0f)) *
                       rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                       scale(mat4(1.0), vec3(0.05f, 1.0f, 0.05f));

    SetUniformMat4(shaderProgram, "world_matrix", xAxisMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram

    // DRAW GREEN CUBE for Y-Axis
    glBindVertexArray(greenAO);
    mat4 yAxisMatrix = translate(mat4(1.0), vec3(0.0, 0.5f, 0.0f)) *
                       scale(mat4(1.0), vec3(0.05f, 1.0f, 0.05f));
    SetUniformMat4(shaderProgram, "world_matrix", yAxisMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram

    // DRAW BLUE CUBE for Z-Axis
    glBindVertexArray(blueAO);
    mat4 zAxisMatrix = translate(mat4(1.0), vec3(0.0f, 0.0f, 0.5f)) *
                       rotate(mat4(1.0), radians(90.0f), vec3(1.0f, 0.0f, 0.0f)) *
                       scale(mat4(1.0), vec3(0.05f, 1.0f, 0.05f));
    SetUniformMat4(shaderProgram, "world_matrix", zAxisMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram
}
//*************************************************************************************************************
// Drawing the Tennis net ----------------------------------------------
void draw_net(GLuint shaderProgram, vec3 translateBy)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);
    // glBindVertexArray(activeVAO);

    int WcubeAO = createTexturedCubeVertexArrayObject(); // Load White Cube
    glBufferData(GL_ARRAY_BUFFER, sizeof(whiteCube), whiteCube, GL_STATIC_DRAW);

    int blackCubeAO = createTexturedCubeVertexArrayObject(); // Load Black Cube
    glBufferData(GL_ARRAY_BUFFER, sizeof(blackCube), blackCube, GL_STATIC_DRAW);

    glBindVertexArray(WcubeAO);

    // Tennis Poles ----------------------------------------------------------------
    vec3 scaledPoles = vec3(0.2f, 1.32f, 0.2f);

    mat4 tennisPoleMatrix = translate(mat4(1.0), translateBy) *
                            translate(mat4(1.0), vec3(0.0f, 0.66f, -4.3f)) *
                            scale(mat4(1.0), scaledPoles);
    SetUniformMat4(shaderProgram, "world_matrix", tennisPoleMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);                   // For shaderProgram
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // For textureProgram
    // glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    tennisPoleMatrix = translate(mat4(1.0), translateBy) *
                       translate(mat4(1.0), vec3(0.0f, 0.66f, 0.0f)) *
                       scale(mat4(1.0), scaledPoles);
    SetUniformMat4(shaderProgram, "world_matrix", tennisPoleMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);                   // For shaderProgram
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // For textureProgram

    // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    tennisPoleMatrix = translate(mat4(1.0), translateBy) *
                       translate(mat4(1.0), vec3(0.0f, 0.66f, 4.3f)) *
                       scale(mat4(1.0), scaledPoles);
    SetUniformMat4(shaderProgram, "world_matrix", tennisPoleMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);                   // For shaderProgram
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // For textureProgram

    // Tennis Net ----------------------------------------------------------------
    glBindVertexArray(blackCubeAO);
    vec3 scaledNet = vec3(0.02f, 0.5f, 0.02f);
    float netY = 0.25f;
    float netZ = -4.0f;
    mat4 tennisNetMatrix = translate(mat4(1.0), vec3(2.5f, 0.625f, -5.0f)) * scale(mat4(1.0), scaledNet);
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 41; ++j)
        {
            // Horizontal lines along the z plane
            tennisNetMatrix = translate(mat4(1.0), translateBy) *
                              translate(mat4(1.0), vec3(0.0f, netY + i * 0.2f, netZ + j * 0.2f)) *
                              rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *
                              rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                              scale(mat4(1.0), scaledNet);
            SetUniformMat4(shaderProgram, "world_matrix", tennisNetMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram

            // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

            // Horizontal lines along the x plane
            tennisNetMatrix = translate(mat4(1.0), translateBy) *
                              translate(mat4(1.0), vec3(0.0f, netY + i * 0.2, netZ + j * 0.2f)) *
                              scale(mat4(1.0), scaledNet);
            SetUniformMat4(shaderProgram, "world_matrix", tennisNetMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram
            // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);
        }
    }
    // Top of Tennis Net
    glBindVertexArray(WcubeAO);
    // // glBindVertexArray(activeVAO);
    tennisNetMatrix = translate(mat4(1.0), translateBy) *
                      translate(mat4(1.0), vec3(0.0f, 1.26f, 0.0f)) *
                      scale(mat4(1.0), vec3(0.025f, 0.1f, 8.6f));
    SetUniformMat4(shaderProgram, "world_matrix", tennisNetMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram
    // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    // Bottom of Tennis net
    glBindVertexArray(blackCubeAO);
    // // glBindVertexArray(activeVAO);
    tennisNetMatrix = translate(mat4(1.0), translateBy) *
                      translate(mat4(1.0), vec3(0.0f, 0.01f, 0.0f)) *
                      scale(mat4(1.0), vec3(0.02, 0.02f, 8.6f));
    SetUniformMat4(shaderProgram, "world_matrix", tennisNetMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram
    // glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);
}
// Drawing a tennis court ----------------------------------------------------------
void draw_court(GLuint shaderProgram, vec3 translateBy)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);

    // Drawing tennis court base color
    int baseAO = createTexturedCubeVertexArrayObject();
    glBufferData(GL_ARRAY_BUFFER, sizeof(courtGround), courtGround, GL_STATIC_DRAW);
    glBindVertexArray(baseAO);

    vec3 scaled = vec3(15.2f, 0.02f, 6.8f);

    mat4 courtMatrix = translate(mat4(1.0), translateBy) *
                       translate(mat4(1.0), vec3(0.0f, -0.05f, 0.0f)) *
                       scale(mat4(1.0), scaled);
    SetUniformMat4(shaderProgram, "world_matrix", courtMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);                   // For shaderProgram
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // For textureProgram
}
// Drawing court lines -------------------------------------------------------------------
void draw_courtLines(GLuint shaderProgram, vec3 translateBy)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);

    int WcubeAO = createTexturedCubeVertexArrayObject();
    glBufferData(GL_ARRAY_BUFFER, sizeof(whiteCube), whiteCube, GL_STATIC_DRAW);
    glBindVertexArray(WcubeAO);

    vec3 scaled_L = vec3(15.3f, 0.02, 0.1f); // Length Scaled (4)
    vec3 scaled_W = vec3(0.1f, 0.02f, 6.8f); // Width Scaled (2)
    vec3 scaled_S = vec3(0.1f, 0.02f, 5.1f); // Service Line Scaled (2)
    vec3 scaled_C = vec3(0.5f, 0.02f, 0.1f); // Center Mark Scaled (2)

    // Lines drawn along the length side
    mat4 lineMatrix = translate(mat4(1.0), translateBy) *
                      translate(mat4(1.0), vec3(0.0f, 0.0f, -3.4f)) *
                      scale(mat4(1.0), scaled_L); // Outer Line in negative Z-Axis
    SetUniformMat4(shaderProgram, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram

    lineMatrix = translate(mat4(1.0), translateBy) *
                 translate(mat4(1.0), vec3(0.0f, 0.0f, -2.55f)) *
                 scale(mat4(1.0), scaled_L); // Inner Line in negative Z-Axis
    SetUniformMat4(shaderProgram, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram

    lineMatrix = translate(mat4(1.0), translateBy) *
                 translate(mat4(1.0), vec3(0.0f, 0.0f, 2.55f)) *
                 scale(mat4(1.0), scaled_L); // Inner Line in positive Z-Axis
    SetUniformMat4(shaderProgram, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram

    lineMatrix = translate(mat4(1.0), translateBy) *
                 translate(mat4(1.0), vec3(0.0f, 0.0f, 3.4f)) *
                 scale(mat4(1.0), scaled_L); // Outer Line in positive Z-Axis
    SetUniformMat4(shaderProgram, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram

    // Lines drawn along the width side
    lineMatrix = translate(mat4(1.0), translateBy) *
                 translate(mat4(1.0), vec3(-7.6f, 0.0f, 0.0f)) *
                 scale(mat4(1.0), scaled_W);
    SetUniformMat4(shaderProgram, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram

    lineMatrix = translate(mat4(1.0), translateBy) *
                 translate(mat4(1.0), vec3(7.6f, 0.0f, 0.0f)) *
                 scale(mat4(1.0), scaled_W);
    SetUniformMat4(shaderProgram, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram

    // Service lines
    lineMatrix = translate(mat4(1.0), translateBy) *
                 translate(mat4(1.0), vec3(-4.09f, 0.0f, 0.0f)) *
                 scale(mat4(1.0), scaled_S);
    SetUniformMat4(shaderProgram, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram

    lineMatrix = translate(mat4(1.0), translateBy) *
                 translate(mat4(1.0), vec3(4.09f, 0.0f, 0.0f)) *
                 scale(mat4(1.0), scaled_S);
    SetUniformMat4(shaderProgram, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram

    // Center Mark lines
    lineMatrix = translate(mat4(1.0), translateBy) *
                 translate(mat4(1.0), vec3(-7.4f, 0.0f, 0.0f)) *
                 scale(mat4(1.0), scaled_C);
    SetUniformMat4(shaderProgram, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram

    lineMatrix = translate(mat4(1.0), translateBy) *
                 translate(mat4(1.0), vec3(7.4f, 0.0f, 0.0f)) *
                 scale(mat4(1.0), scaled_C);
    SetUniformMat4(shaderProgram, "world_matrix", lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram
}
//*************************************************************************************************************
// Drawing a world ground
void draw_ground(GLuint shaderProgram, vec3 translateBy)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);
    int groundAO = createTexturedCubeVertexArrayObject();
    glBufferData(GL_ARRAY_BUFFER, sizeof(brownCube), brownCube, GL_STATIC_DRAW);
    glBindVertexArray(groundAO);

    mat4 groundMatrix = translate(mat4(1.0), translateBy) *
                        translate(mat4(1.0), vec3(0.0f, -0.06f, 0.0f)) *
                        scale(mat4(1.0), vec3(25.0f, 0.02f, 12.0f));
    SetUniformMat4(shaderProgram, "world_matrix", groundMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);                   // For shaderProgram
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // For textureProgram
}
//*************************************************************************************************************
// Drawing the Letter Characters -------------------------------------
/****** V ******/
void draw_V(GLuint shaderProgram, vec3 translateBy, float degrees, vec3 rotateBy, vec3 scaleBy)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);
    // glBindVertexArray(activeVAO);

    // int VcubeAO = createVertexBufferObject();
    // glBindVertexArray(VcubeAO);
    SetUniformVec3(shaderProgram, "object_color", colors[1]);

    vec3 scaled = vec3(0.2f, 1.5f, 0.2f);

    vec3 worldScaled = vec3(0.25f, 0.25f, 0.25f);

    mat4 VcubeMatrix1 = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateBy) * scale(mat4(1.0), worldScaled) *
                        translate(mat4(1.0), vec3(-1.0f, 1.0f, 0.0f)) *
                        rotate(mat4(1.0), radians(30.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), scaled);
    SetUniformMat4(shaderProgram, "world_matrix", VcubeMatrix1);
    SetUniformVec3(shaderProgram, "object_color", colors[1]);

    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram
    // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    mat4 VcubeMatrix2 = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateBy) * scale(mat4(1.0), worldScaled) *
                        translate(mat4(1.0), vec3(-0.35f, 1.0f, 0.0f)) *
                        rotate(mat4(1.0), radians(-30.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), scaled);
    SetUniformMat4(shaderProgram, "world_matrix", VcubeMatrix2);
    SetUniformVec3(shaderProgram, "object_color", colors[1]);

    glDrawArrays(GL_TRIANGLES, 0, 36); // For shaderProgram
    // glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    translateBy = vec3(-3.5f, 0.5f, -1.7);

    draw_racket(shaderProgram, translateBy, degrees, rotateBy, scaleBy, renderMode);
}
//*************************************************************************************************************
// Drawing the Tennis Racket -----------------------------------------
void draw_racket(GLuint shaderProgram, vec3 translateBy, float degrees, vec3 rotateBy, vec3 scaleBy, GLenum renderMode)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);

    // Create a scalar vector as base reference
    vec3 scalar = vec3(0.2f, 0.2f, 0.2f);

    // Center Net of Racket -----------------------------------------------------------
    SetUniformVec3(shaderProgram, "object_color", colors[3]); // Set active color to Green
    // using a for-loop for grid pattern
    mat4 racketNet = mat4(1.0);

    vec3 RACKET_NET_SCALED = vec3(0.02f, 0.45f, 0.02f);
    float RACKET_NET_WIDTH = 0.8f;
    float RACKET_NET_HEIGHT = 5.45f;

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 11; ++j)
        {
            // Horizontal lines
            racketNet = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateBy) * scale(mat4(1.0), scaleBy) *
                        translate(mat4(1.0), vec3(RACKET_NET_WIDTH + i * 0.2f, RACKET_NET_HEIGHT + j * 0.2, 0.0f)) *
                        rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), RACKET_NET_SCALED);
            SetUniformMat4(shaderProgram, "world_matrix", racketNet);
            glDrawArrays(renderMode, 0, 36);

            // Vertcal lines
            racketNet = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateBy) * scale(mat4(1.0), scaleBy) *
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
    mat4 leftTop = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateBy) * scale(mat4(1.0), scaleBy) *
                   scale(mat4(1.0), scalar) *
                   translate(mat4(1.0), vec3(0.95f, TOP_SIDES_HEIGHT, 0.0f)) *
                   rotate(mat4(1.0), radians(-65.0f), vec3(0.0f, 0.0f, 1.0f)) *
                   scale(mat4(1.0), TOP_SIDES_SCALED);
    SetUniformMat4(shaderProgram, "world_matrix", leftTop);
    glDrawArrays(renderMode, 0, 36);
    glDrawElements(renderMode, 6, GL_UNSIGNED_INT, 0);

    // right top
    mat4 rightTop = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateBy) * scale(mat4(1.0), scaleBy) *
                    scale(mat4(1.0), scalar) *
                    translate(mat4(1.0), vec3(2.05f, TOP_SIDES_HEIGHT, 0.0f)) *
                    rotate(mat4(1.0), radians(65.0f), vec3(0.0f, 0.0f, 1.0f)) *
                    scale(mat4(1.0), TOP_SIDES_SCALED);
    SetUniformMat4(shaderProgram, "world_matrix", rightTop);
    glDrawArrays(renderMode, 0, 36);
    glDrawElements(renderMode, 6, GL_UNSIGNED_INT, 0);

    // middle top
    SetUniformVec3(shaderProgram, "object_color", colors[6]); // Set active color Red
    mat4 middleTop = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateBy) * scale(mat4(1.0), scaleBy) *
                     scale(mat4(1.0), scalar) *
                     translate(mat4(1.0), vec3(1.5f, 7.8f, 0.0f)) *
                     rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                     scale(mat4(1.0), vec3(0.17f, 0.4f, 0.17f));
    SetUniformMat4(shaderProgram, "world_matrix", middleTop);
    glDrawArrays(renderMode, 0, 36);
    glDrawElements(renderMode, 6, GL_UNSIGNED_INT, 0);

    // Sides of Racket ---------------------------------------------------
    vec3 SIDES_SCALED = vec3(0.17f, 2.0f, 0.17f);
    float SIDES_HEIGHT = 6.5f;

    // left side
    mat4 leftSide = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateBy) * scale(mat4(1.0), scaleBy) *
                    scale(mat4(1.0), scalar) *
                    translate(mat4(1.0), vec3(0.6f, SIDES_HEIGHT, 0.0f)) *
                    scale(mat4(1.0), SIDES_SCALED);
    SetUniformMat4(shaderProgram, "world_matrix", leftSide);
    glDrawArrays(renderMode, 0, 36);
    glDrawElements(renderMode, 6, GL_UNSIGNED_INT, 0);

    // right side
    mat4 rightSide = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateBy) * scale(mat4(1.0), scaleBy) *
                     scale(mat4(1.0), scalar) *
                     translate(mat4(1.0), vec3(2.4f, SIDES_HEIGHT, 0.0f)) *
                     scale(mat4(1.0), SIDES_SCALED);
    SetUniformMat4(shaderProgram, "world_matrix", rightSide);
    glDrawArrays(renderMode, 0, 36);
    glDrawElements(renderMode, 6, GL_UNSIGNED_INT, 0);

    // Base of Racket ------------------------------------------------------
    vec3 BASE_SIDES_SCALED = vec3(0.17f, 1.25f, 0.17f);
    float BASE_SIDES_HEIGHT = 5.1f;

    // middle base
    mat4 middleBase = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateBy) * scale(mat4(1.0), scaleBy) *
                      scale(mat4(1.0), scalar) * translate(mat4(1.0), vec3(1.5f, 5.15f, 0.0f)) *
                      rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                      scale(mat4(1.0), vec3(0.15f, 1.1f, 0.15f));
    SetUniformMat4(shaderProgram, "world_matrix", middleBase);
    glDrawArrays(renderMode, 0, 36);
    glDrawElements(renderMode, 6, GL_UNSIGNED_INT, 0);

    // left base
    SetUniformVec3(shaderProgram, "object_color", colors[7]); // Set active color Gray

    mat4 leftBase = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateBy) * scale(mat4(1.0), scaleBy) *
                    scale(mat4(1.0), scalar) * translate(mat4(1.0), vec3(1.05f, BASE_SIDES_HEIGHT, 0.0f)) *
                    rotate(mat4(1.0), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                    scale(mat4(1.0), BASE_SIDES_SCALED);
    SetUniformMat4(shaderProgram, "world_matrix", leftBase);
    glDrawArrays(renderMode, 0, 36);
    glDrawElements(renderMode, 6, GL_UNSIGNED_INT, 0);

    // right base
    mat4 rightBase = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateBy) * scale(mat4(1.0), scaleBy) *
                     scale(mat4(1.0), scalar) * translate(mat4(1.0), vec3(1.95f, BASE_SIDES_HEIGHT, 0.0f)) *
                     rotate(mat4(1.0), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                     scale(mat4(1.0), BASE_SIDES_SCALED);
    SetUniformMat4(shaderProgram, "world_matrix", rightBase);
    glDrawArrays(renderMode, 0, 36);
    glDrawElements(renderMode, 6, GL_UNSIGNED_INT, 0);

    // Racket Handle --------------------------------------------------
    SetUniformVec3(shaderProgram, "object_color", colors[6]); // Set active color Red
    mat4 handleRacket = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateBy) * scale(mat4(1.0), scaleBy) *
                        scale(mat4(1.0), scalar) *
                        translate(mat4(1.0), vec3(1.5f, 3.75f, 0.0f)) *
                        rotate(mat4(1.0), radians(0.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), vec3(0.15f, 2.0f, 0.15f));
    SetUniformMat4(shaderProgram, "world_matrix", handleRacket);
    glDrawArrays(renderMode, 0, 36);
    glDrawElements(renderMode, 6, GL_UNSIGNED_INT, 0);

    draw_arm(shaderProgram, translateBy, degrees, rotateBy, scaleBy, renderMode);
}
//*************************************************************************************************************
// DRAWING the ARM ---------------------------------------------
void draw_arm(GLuint shaderProgram, vec3 translateBy, float degrees, vec3 rotateBy, vec3 scaleBy, GLenum renderMode)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);

    // Create a scalar vector as base reference
    vec3 scalar = vec3(0.2f, 0.2f, 0.2f);

    SetUniformVec3(shaderProgram, "object_color", colors[8]); // Set active color Beige for Arm
    vec3 ARM_SCALED = vec3(0.25f, 1.5f, 0.25f);

    // Lower Arm
    mat4 lowerArmMatrix = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateBy) * scale(mat4(1.0), scaleBy) *
                          scale(mat4(1.0), scalar) *
                          translate(mat4(1.0), vec3(1.0f, 1.0f, 0.0f)) *
                          rotate(mat4(1.0), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                          scale(mat4(1.0), ARM_SCALED);
    SetUniformMat4(shaderProgram, "world_matrix", lowerArmMatrix);
    glDrawArrays(renderMode, 0, 36);
    glDrawElements(renderMode, 6, GL_UNSIGNED_INT, 0);

    // Upper Arm
    mat4 upperArmMatrix = translate(mat4(1.0), translateBy) * rotate(mat4(1.0), radians(degrees), rotateBy) * scale(mat4(1.0), scaleBy) *
                          scale(mat4(1.0), scalar) *
                          translate(mat4(1.0), vec3(1.5f, 2.25f, 0.0f)) *
                          rotate(mat4(1.0), radians(0.0f), vec3(0.0f, 0.0f, 1.0f)) *
                          scale(mat4(1.0), ARM_SCALED);
    SetUniformMat4(shaderProgram, "world_matrix", upperArmMatrix);
    glDrawArrays(renderMode, 0, 36);
}
//*************************************************************************************************************
// Set up for a crowd into environment
void draw_crowd(GLuint shaderProgram, vec3 translateBy, float degrees, vec3 rotation)
{
    // Note: Single Court Ground Base is 25 units by 12 units
    // Compile and link shaders
    glUseProgram(shaderProgram);

    int cubeAO = createTexturedCubeVertexArrayObject();
    glBufferData(GL_ARRAY_BUFFER, sizeof(whiteCube), whiteCube, GL_STATIC_DRAW);
    glBindVertexArray(cubeAO);

    // Drawing the crowd stands in the positive x-plane
    mat4 crowd = translate(mat4(1.0), translateBy) *
                 translate(mat4(1.0), vec3(0.0f, 2.0f, 0.0f)) *
                 rotate(mat4(1.0), radians(degrees), rotation) *
                 rotate(mat4(1.0), radians(-240.0f), vec3(1.0f, 0.0f, 0.0f)) *
                 scale(mat4(1.0), vec3(8.0, 8.0, 0.05f));
    SetUniformMat4(shaderProgram, "world_matrix", crowd);
    glDrawArrays(GL_TRIANGLES, 0, 36);                   // For shaderProgram
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // For textureProgram
}
//*************************************************************************************************************
// Drawing a scoreboard
void draw_scoreboard(GLuint shaderProgram, vec3 translateBy, vec3 scaleBy)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);

    int cubeAO = createTexturedCubeVertexArrayObject();
    glBufferData(GL_ARRAY_BUFFER, sizeof(blackCube), blackCube, GL_STATIC_DRAW);
    glBindVertexArray(cubeAO);

    // Drawing the crowd stands in the positive x-plane
    mat4 scoreboard = translate(mat4(1.0), translateBy) *
                      rotate(mat4(1.0), radians(270.0f), vec3(0.0f, 1.0f, 0.0f)) *
                      rotate(mat4(1.0), radians(180.0f), vec3(1.0f, 0.0f, 0.0f)) *
                      scale(mat4(1.0), scaleBy);
    SetUniformMat4(shaderProgram, "world_matrix", scoreboard);
    glDrawArrays(GL_TRIANGLES, 0, 36);                   // For shaderProgram
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // For textureProgram
}

#endif /* MODELOBJECTS_H */
