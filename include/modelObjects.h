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

#include "lightFrag.h"
#include "lightVert.h"

using namespace glm;
using namespace std;

// For Changing the rendering of the geometric shapes
float polygons = GL_TRIANGLES;

void draw_light(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation,  GLuint activeVAO, int activeVertices);

// OBJECT CREATIONS ---------------------------------------
void draw_grid(int cubeAO, int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation);

void draw_net(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, GLuint activeVAO, int activeVertices);
void draw_court(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation);
void draw_courtLines(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation);
void draw_ground(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation);

void draw_V(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, float degrees, vec3 worldRotation, vec3 worldScale, GLuint activeVAO, int activeVertices);
void draw_A(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, float degrees, vec3 worldRotation, vec3 worldScale, GLuint activeVAO, int activeVertices);
void draw_L(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, float degrees, vec3 worldRotation, vec3 worldScale, GLuint activeVAO, int activeVertices);
void draw_D(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, float degrees, vec3 worldRotation, vec3 worldScale, GLuint activeVAO, int activeVertices);

void draw_racket(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, float degrees, vec3 worldRotation, vec3 worldScale, float polygons, GLuint activeVAO, int activeVertices);
void draw_arm(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, float degrees, vec3 worldRotation, vec3 worldScale, float polygons, GLuint activeVAO, int activeVertices);

void draw_ball(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, float polygons, GLuint activeVAO, int activeVertices);

/********************************************************************************************************/
/**************************
 * DRAWING A LIGHT SOURCE *
 **************************/
void draw_light(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation,  GLuint activeVAO, int activeVertices)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);
    // glBindVertexArray(activeVAO);

    int lightAO = createTexturedCubeVertexArrayObject();
    glBufferData(GL_ARRAY_BUFFER, sizeof(whiteCube), whiteCube, GL_STATIC_DRAW);

    mat4 lightMatrix = translate(mat4(1.0), worldTranslation);
    setWorldMatrix(shaderProgram, lightMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

}
/********************************************************************************************************/
/*************************
 * DRAWING MODEL OBJECTS *
 *************************/
// DRAW GROUND GRID and AXIS ORIGIN ------------------------------------------------------------------
void draw_grid(int texturedCubeVAO, int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);

    // Define and upload geometry to the GPU
    glBindVertexArray(texturedCubeVAO);

    vec3 GROUND_GRID_SCALED = vec3(0.01f, 0.5f, 0.01f);
    worldTranslation = vec3(-2.8f, 0.0f, 1.4f);
    mat4 groundWorldMatrix = mat4(1.0);

    for (int i = 0; i < 78; ++i) // Along the X axis: -4.8 -> 10.4,
    {
        for (int j = 0; j < 36; ++j) // Along the Z axis: -4.8 -> 2
        {
            // Horizontal lines along the z plane
            groundWorldMatrix = translate(mat4(1.0), worldTranslation) *
                                translate(mat4(1.0), vec3(-5.0f + i * 0.2f, 0.0f, -5.0f + j * 0.2f)) *
                                rotate(mat4(1.0), radians(90.0f), vec3(1.0f, 0.0f, 0.0f)) *
                                scale(mat4(1.0), GROUND_GRID_SCALED);
            setWorldMatrix(shaderProgram, groundWorldMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Horizontal lines along the x plane
            groundWorldMatrix = translate(mat4(1.0), worldTranslation) *
                                translate(mat4(1.0), vec3(-5.0f + i * 0.2, 0.0f, -5.0f + j * 0.2f)) *
                                rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                                scale(mat4(1.0), GROUND_GRID_SCALED);
            setWorldMatrix(shaderProgram, groundWorldMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36);
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

    setWorldMatrix(shaderProgram, xAxisMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // DRAW GREEN CUBE for Y-Axis
    glBindVertexArray(greenAO);
    mat4 yAxisMatrix = translate(mat4(1.0), vec3(0.0, 0.5f, 0.0f)) *
                       scale(mat4(1.0), vec3(0.05f, 1.0f, 0.05f));
    setWorldMatrix(shaderProgram, yAxisMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // DRAW BLUE CUBE for Z-Axis
    glBindVertexArray(blueAO);
    mat4 zAxisMatrix = translate(mat4(1.0), vec3(0.0f, 0.0f, 0.5f)) *
                       rotate(mat4(1.0), radians(90.0f), vec3(1.0f, 0.0f, 0.0f)) *
                       scale(mat4(1.0), vec3(0.05f, 1.0f, 0.05f));
    setWorldMatrix(shaderProgram, zAxisMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
//*************************************************************************************************************
// Drawing the Tennis net ----------------------------------------------
void draw_net(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, GLuint activeVAO, int activeVertices)
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

    mat4 tennisPoleMatrix = translate(mat4(1.0), worldTranslation) *
                            translate(mat4(1.0), vec3(0.0f, 0.66f, -4.3f)) *
                            scale(mat4(1.0), scaledPoles);
    setWorldMatrix(shaderProgram, tennisPoleMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    tennisPoleMatrix = translate(mat4(1.0), worldTranslation) *
                            translate(mat4(1.0), vec3(0.0f, 0.66f, 0.0f)) *
                            scale(mat4(1.0), scaledPoles);
    setWorldMatrix(shaderProgram, tennisPoleMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    tennisPoleMatrix = translate(mat4(1.0), worldTranslation) *
                       translate(mat4(1.0), vec3(0.0f, 0.66f, 4.3f)) *
                       scale(mat4(1.0), scaledPoles);
    setWorldMatrix(shaderProgram, tennisPoleMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    // Tennis Net ----------------------------------------------------------------
    glBindVertexArray(blackCubeAO);
    // // glBindVertexArray(activeVAO);
    vec3 scaledNet = vec3(0.02f, 0.5f, 0.02f);
    float netY = 0.25f;
    float netZ = -4.0f;
    mat4 tennisNetMatrix = translate(mat4(1.0), vec3(2.5f, 0.625f, -5.0f)) * scale(mat4(1.0), scaledNet);
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 41; ++j)
        {
            // Horizontal lines along the z plane
            tennisNetMatrix = translate(mat4(1.0), worldTranslation) *
                              translate(mat4(1.0), vec3(0.0f, netY + i * 0.2f, netZ + j * 0.2f)) *
                              rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *
                              rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                              scale(mat4(1.0), scaledNet);
            setWorldMatrix(shaderProgram, tennisNetMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

            // Horizontal lines along the x plane
            tennisNetMatrix = translate(mat4(1.0), worldTranslation) *
                              translate(mat4(1.0), vec3(0.0f, netY + i * 0.2, netZ + j * 0.2f)) *
                              scale(mat4(1.0), scaledNet);
            setWorldMatrix(shaderProgram, tennisNetMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);
        }
    }
    // Top of Tennis Net
    glBindVertexArray(WcubeAO);
    // // glBindVertexArray(activeVAO);
    tennisNetMatrix = translate(mat4(1.0), worldTranslation) *
                      translate(mat4(1.0), vec3(0.0f, 1.26f, 0.0f)) *
                      scale(mat4(1.0), vec3(0.025f, 0.1f, 8.6f));
    setWorldMatrix(shaderProgram, tennisNetMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    // Bottom of Tennis net
    glBindVertexArray(blackCubeAO);
    // // glBindVertexArray(activeVAO);
    tennisNetMatrix = translate(mat4(1.0), worldTranslation) *
                      translate(mat4(1.0), vec3(0.0f, 0.01f, 0.0f)) *
                      scale(mat4(1.0), vec3(0.02, 0.02f, 8.6f));
    setWorldMatrix(shaderProgram, tennisNetMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);
}
// Drawing a tennis court ----------------------------------------------------------
void draw_court(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);

    // Drawing tennis court base color
    int baseAO = createTexturedCubeVertexArrayObject();
    glBufferData(GL_ARRAY_BUFFER, sizeof(courtGround), courtGround, GL_STATIC_DRAW);
    glBindVertexArray(baseAO);

    vec3 scaled = vec3(15.2f, 0.02f, 6.8f);

    mat4 courtMatrix = translate(mat4(1.0), worldTranslation) *
                       translate(mat4(1.0), vec3(0.0f, -0.05f, 0.0f)) *
                       scale(mat4(1.0), scaled);
    setWorldMatrix(shaderProgram, courtMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
// Drawing court lines -------------------------------------------------------------------
void draw_courtLines(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation)
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
    mat4 lineMatrix = translate(mat4(1.0), worldTranslation) *
                      translate(mat4(1.0), vec3(0.0f, 0.0f, -3.4f)) *
                      scale(mat4(1.0), scaled_L); // Outer Line in negative Z-Axis
    setWorldMatrix(shaderProgram, lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lineMatrix = translate(mat4(1.0), worldTranslation) *
                 translate(mat4(1.0), vec3(0.0f, 0.0f, -2.55f)) *
                 scale(mat4(1.0), scaled_L); // Inner Line in negative Z-Axis
    setWorldMatrix(shaderProgram, lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lineMatrix = translate(mat4(1.0), worldTranslation) *
                 translate(mat4(1.0), vec3(0.0f, 0.0f, 2.55f)) *
                 scale(mat4(1.0), scaled_L); // Inner Line in positive Z-Axis
    setWorldMatrix(shaderProgram, lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lineMatrix = translate(mat4(1.0), worldTranslation) *
                 translate(mat4(1.0), vec3(0.0f, 0.0f, 3.4f)) *
                 scale(mat4(1.0), scaled_L); // Outer Line in positive Z-Axis
    setWorldMatrix(shaderProgram, lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Lines drawn along the width side
    lineMatrix = translate(mat4(1.0), worldTranslation) *
                 translate(mat4(1.0), vec3(-7.6f, 0.0f, 0.0f)) *
                 scale(mat4(1.0), scaled_W);
    setWorldMatrix(shaderProgram, lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lineMatrix = translate(mat4(1.0), worldTranslation) *
                 translate(mat4(1.0), vec3(7.6f, 0.0f, 0.0f)) *
                 scale(mat4(1.0), scaled_W);
    setWorldMatrix(shaderProgram, lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Service lines
    lineMatrix = translate(mat4(1.0), worldTranslation) *
                 translate(mat4(1.0), vec3(-4.09f, 0.0f, 0.0f)) *
                 scale(mat4(1.0), scaled_S);
    setWorldMatrix(shaderProgram, lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lineMatrix = translate(mat4(1.0), worldTranslation) *
                 translate(mat4(1.0), vec3(4.09f, 0.0f, 0.0f)) *
                 scale(mat4(1.0), scaled_S);
    setWorldMatrix(shaderProgram, lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Center Mark lines
    lineMatrix = translate(mat4(1.0), worldTranslation) *
                 translate(mat4(1.0), vec3(-7.4f, 0.0f, 0.0f)) *
                 scale(mat4(1.0), scaled_C);
    setWorldMatrix(shaderProgram, lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lineMatrix = translate(mat4(1.0), worldTranslation) *
                 translate(mat4(1.0), vec3(7.4f, 0.0f, 0.0f)) *
                 scale(mat4(1.0), scaled_C);
    setWorldMatrix(shaderProgram, lineMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
//*************************************************************************************************************
// Drawing a world ground
void draw_ground(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);
    int groundAO = createTexturedCubeVertexArrayObject();
    glBufferData(GL_ARRAY_BUFFER, sizeof(brownCube), brownCube, GL_STATIC_DRAW);
    glBindVertexArray(groundAO);

    mat4 groundMatrix = translate(mat4(1.0), worldTranslation) *
                        translate(mat4(1.0), vec3(0.0f, -0.06f, 0.0f)) *
                        scale(mat4(1.0), vec3(25.0f, 0.02f, 12.0f));
    setWorldMatrix(shaderProgram, groundMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
//*************************************************************************************************************
// Drawing the Letter Characters -------------------------------------
/****** V ******/
void draw_V(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, float degrees, vec3 worldRotation, vec3 worldScale, GLuint activeVAO, int activeVertices)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);
    // glBindVertexArray(activeVAO);

    int VcubeAO = createVertexBufferObject();
    glBindVertexArray(VcubeAO);

    vec3 scaled = vec3(0.2f, 1.5f, 0.2f);

    vec3 worldScaled = vec3(0.25f, 0.25f, 0.25f);

    mat4 VcubeMatrix1 = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScaled) *
                        translate(mat4(1.0), vec3(-1.0f, 1.0f, 0.0f)) *
                        rotate(mat4(1.0), radians(30.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), scaled);
    setWorldMatrix(shaderProgram, VcubeMatrix1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    mat4 VcubeMatrix2 = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScaled) *
                        translate(mat4(1.0), vec3(-0.35f, 1.0f, 0.0f)) *
                        rotate(mat4(1.0), radians(-30.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), scaled);
    setWorldMatrix(shaderProgram, VcubeMatrix2);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    worldTranslation = vec3(-3.5f, 0.5f, -1.7);

    draw_racket(shaderProgram, worldMatrixLocation, worldTranslation, degrees, worldRotation, worldScale, polygons, activeVAO, activeVertices);
}
//------------------------------------------------------------------------------
/****** A ******/
void draw_A(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, float degrees, vec3 worldRotation, vec3 worldScale, GLuint activeVAO, int activeVertices)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);
    // glBindVertexArray(activeVAO);

    int AcubeAO = createVertexBufferObject();
    glBindVertexArray(AcubeAO);
    vec3 scaled = vec3(0.2f, 1.5f, 0.2f);

    vec3 worldScaled = vec3(0.25f, 0.25f, 0.25f);

    mat4 AcubeMatrix1 = translate(mat4(1.0), worldTranslation)  * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScaled) *
                        translate(mat4(1.0), vec3(-1.0f, 1.0f, 0.0f)) *
                        rotate(mat4(1.0), radians(-30.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), scaled);
    setWorldMatrix(shaderProgram, AcubeMatrix1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    mat4 AcubeMatrix2 = translate(mat4(1.0), worldTranslation)  * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScaled) *
                        translate(mat4(1.0), vec3(-0.35f, 1.0f, 0.0f)) *
                        rotate(mat4(1.0), radians(30.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), scaled);
    setWorldMatrix(shaderProgram, AcubeMatrix2);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    mat4 AcubeMatrix3 = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScaled) *
                        translate(mat4(1.0), vec3(-0.7f, 0.75f, 0.0f)) *
                        rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), scaled);
    setWorldMatrix(shaderProgram, AcubeMatrix3);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    worldTranslation = vec3(-3.5f, 0.5f, 1.7);
    draw_racket(shaderProgram, worldMatrixLocation, worldTranslation, degrees, worldRotation, worldScale, polygons, activeVAO, activeVertices);
}
//-------------------------------------------------------------------------------
/***** L ******/
void draw_L(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, float degrees, vec3 worldRotation, vec3 worldScale, GLuint activeVAO, int activeVertices)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);
    // glBindVertexArray(activeVAO);

    int LcubeAO = createVertexBufferObject();
    glBindVertexArray(LcubeAO);
    vec3 scaled = vec3(0.2f, 1.5f, 0.2f);
    vec3 scaled2 = vec3(0.2f, 1.0f, 0.2f);

    vec3 worldScaled = vec3(0.25f, 0.25f, 0.25f);

    mat4 LcubeMatrix1 = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScaled) *
                        translate(mat4(1.0), vec3(1.0f, 1.0f, 0.0f)) *
                        scale(mat4(1.0), scaled);
    setWorldMatrix(shaderProgram, LcubeMatrix1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    mat4 LcubeMatrix2 = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScaled) *
                        translate(mat4(1.0), vec3(1.5f, 0.35f, 0.0f)) *
                        rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), scaled2);
    setWorldMatrix(shaderProgram, LcubeMatrix2);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    worldTranslation = vec3(3.0f, 0.5f, -1.7);
    draw_racket(shaderProgram, worldMatrixLocation, worldTranslation, degrees, worldRotation, worldScale, polygons, activeVAO, activeVertices);
}
//-------------------------------------------------------------------------------
/****** D ******/
void draw_D(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, float degrees, vec3 worldRotation, vec3 worldScale, GLuint activeVAO, int activeVertices)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);
    // glBindVertexArray(activeVAO);

    int DcubeAO = createVertexBufferObject();
    glBindVertexArray(DcubeAO);

    vec3 scaled = vec3(0.2f, 1.5f, 0.2f);
    vec3 scaled2 = vec3(0.2f, 1.0f, 0.2f);
    vec3 scaled3 = vec3(0.2f, 1.25f, 0.2f);

    vec3 worldScaled = vec3(0.25f, 0.25f, 0.25f);

    mat4 DcubeMatrix1 = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScaled) *
                        translate(mat4(1.0), vec3(1.0f, 1.0f, 0.0)) *
                        scale(mat4(1.0), scaled);
    setWorldMatrix(shaderProgram, DcubeMatrix1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    mat4 DcubeMatrix2 = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScaled) *
                        translate(mat4(1.0), vec3(1.45f, 1.65f, 0.0f)) *
                        rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), scaled2);
    setWorldMatrix(shaderProgram, DcubeMatrix2);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    mat4 DcubeMatrix3 = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScaled) *
                        translate(mat4(1.0), vec3(1.45f, 0.35f, 0.0f)) *
                        rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), scaled2);
    setWorldMatrix(shaderProgram, DcubeMatrix3);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    mat4 DcubeMatrix4 = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScaled) *
                        translate(mat4(1.0), vec3(2.0f, 1.0f, 0.0f)) *
                        scale(mat4(1.0), scaled3);
    setWorldMatrix(shaderProgram, DcubeMatrix4);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);

    worldTranslation = vec3(3.0f, 0.5f, 1.7);
    draw_racket(shaderProgram, worldMatrixLocation, worldTranslation, degrees, worldRotation, worldScale, polygons, activeVAO, activeVertices);
}
//*************************************************************************************************************
// Drawing the Tennis Racket -----------------------------------------
void draw_racket(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, float degrees, vec3 worldRotation, vec3 worldScale, float polygons, GLuint activeVAO, int activeVertices)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);
    // glBindVertexArray(activeVAO);

    // Create a scalar vector as base reference
    vec3 scalar = vec3(0.2f, 0.2f, 0.2f);

    int greenAO = createTexturedCubeVertexArrayObject(); // Green cube for racket net
    glBufferData(GL_ARRAY_BUFFER, sizeof(greenCube), greenCube, GL_STATIC_DRAW);

    int racketRedAO = createTexturedCubeVertexArrayObject(); // Dark Red Cube
    glBufferData(GL_ARRAY_BUFFER, sizeof(racketRed), racketRed, GL_STATIC_DRAW);

    int racketGrayAO = createTexturedCubeVertexArrayObject(); // Gray Cube
    glBufferData(GL_ARRAY_BUFFER, sizeof(racketGray), racketGray, GL_STATIC_DRAW);

    // Center Net of Racket -----------------------------------------------------------
    glBindVertexArray(greenAO); // Set using Green Cube
    // using a for-loop for grid pattern
    mat4 racketNet = translate(mat4(1.0), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0), vec3(1.0f, 1.0f, 1.0f));

    vec3 RACKET_NET_SCALED = vec3(0.02f, 0.45f, 0.02f);
    float RACKET_NET_X = 0.8f;
    float RACKET_NET_HEIGHT = 5.45f;

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 11; ++j)
        {
            // Horizontal lines
            racketNet = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScale) *
                        scale(mat4(1.0), scalar) *
                        translate(mat4(1.0), vec3(RACKET_NET_X + i * 0.2f, RACKET_NET_HEIGHT + j * 0.2, 0.0f)) *
                        rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), RACKET_NET_SCALED);
            setWorldMatrix(shaderProgram, racketNet);
            glDrawArrays(polygons, 0, 36);
            // glDrawElements(polygons, activeVertices, GL_UNSIGNED_INT, 0);

            // Vertcal lines
            racketNet = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScale) *
                        scale(mat4(1.0), scalar) *
                        translate(mat4(1.0), vec3(RACKET_NET_X + i * 0.2f, RACKET_NET_HEIGHT + j * 0.2f, 0.0f)) *
                        scale(mat4(1.0), RACKET_NET_SCALED);
            setWorldMatrix(shaderProgram, racketNet);
            glDrawArrays(polygons, 0, 36);
            // glDrawElements(polygons, activeVertices, GL_UNSIGNED_INT, 0);
        }
    }

    // Upper racket --------------------------------------------------
    glBindVertexArray(racketGrayAO); // Set using Racket Gray Cube
    // // glBindVertexArray(activeVAO);

    vec3 TOP_SIDES_SCALED = vec3(0.17f, 0.85f, 0.17f);
    float TOP_SIDES_HEIGHT = 7.625f;

    // left top
    mat4 leftTop = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScale) *
                   scale(mat4(1.0), scalar) *
                   translate(mat4(1.0), vec3(0.95f, TOP_SIDES_HEIGHT, 0.0f)) *
                   rotate(mat4(1.0), radians(-65.0f), vec3(0.0f, 0.0f, 1.0f)) *
                   scale(mat4(1.0), TOP_SIDES_SCALED);
    setWorldMatrix(shaderProgram, leftTop);
    glDrawArrays(polygons, 0, 36);
    glDrawElements(polygons, 6, GL_UNSIGNED_INT, 0);
    // glDrawElements(polygons, activeVertices, GL_UNSIGNED_INT, 0);

    // right top
    mat4 rightTop = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScale) *
                    scale(mat4(1.0), scalar) *
                    translate(mat4(1.0), vec3(2.05f, TOP_SIDES_HEIGHT, 0.0f)) *
                    rotate(mat4(1.0), radians(65.0f), vec3(0.0f, 0.0f, 1.0f)) *
                    scale(mat4(1.0), TOP_SIDES_SCALED);
    setWorldMatrix(shaderProgram, rightTop);
    glDrawArrays(polygons, 0, 36);
    glDrawElements(polygons, 6, GL_UNSIGNED_INT, 0);
    // glDrawElements(polygons, activeVertices, GL_UNSIGNED_INT, 0);

    // middle top
    glBindVertexArray(racketRedAO); // Set using Racket Red Cube
    // // glBindVertexArray(activeVAO);
    mat4 middleTop = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScale) *
                     scale(mat4(1.0), scalar) *
                     translate(mat4(1.0), vec3(1.5f, 7.8f, 0.0f)) *
                     rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                     scale(mat4(1.0), vec3(0.17f, 0.4f, 0.17f));
    setWorldMatrix(shaderProgram, middleTop);
    glDrawArrays(polygons, 0, 36);
    glDrawElements(polygons, 6, GL_UNSIGNED_INT, 0);
    // glDrawElements(polygons, activeVertices, GL_UNSIGNED_INT, 0);

    // Sides of Racket ---------------------------------------------------
    vec3 SIDES_SCALED = vec3(0.17f, 2.0f, 0.17f);
    float SIDES_HEIGHT = 6.5f;

    // left side
    mat4 leftSide = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScale) *
                    scale(mat4(1.0), scalar) *
                    translate(mat4(1.0), vec3(0.6f, SIDES_HEIGHT, 0.0f)) *
                    scale(mat4(1.0), SIDES_SCALED);
    setWorldMatrix(shaderProgram, leftSide);
    glDrawArrays(polygons, 0, 36);
    glDrawElements(polygons, 6, GL_UNSIGNED_INT, 0);
    // glDrawElements(polygons, activeVertices, GL_UNSIGNED_INT, 0);

    // right side
    mat4 rightSide = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScale) *
                     scale(mat4(1.0), scalar) *
                     translate(mat4(1.0), vec3(2.4f, SIDES_HEIGHT, 0.0f)) *
                     scale(mat4(1.0), SIDES_SCALED);
    setWorldMatrix(shaderProgram, rightSide);
    glDrawArrays(polygons, 0, 36);
    glDrawElements(polygons, 6, GL_UNSIGNED_INT, 0);
    // glDrawElements(polygons, activeVertices, GL_UNSIGNED_INT, 0);

    // Base of Racket ------------------------------------------------------
    vec3 BASE_SIDES_SCALED = vec3(0.17f, 1.25f, 0.17f);
    float BASE_SIDES_HEIGHT = 5.1f;

    // middle base
    mat4 middleBase = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScale) *
                      scale(mat4(1.0), scalar) * translate(mat4(1.0), vec3(1.5f, 5.15f, 0.0f)) *
                      rotate(mat4(1.0), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) *
                      scale(mat4(1.0), vec3(0.15f, 1.1f, 0.15f));
    setWorldMatrix(shaderProgram, middleBase);
    glDrawArrays(polygons, 0, 36);
    glDrawElements(polygons, 6, GL_UNSIGNED_INT, 0);
    // glDrawElements(polygons, activeVertices, GL_UNSIGNED_INT, 0);

    // left base
    glBindVertexArray(racketGrayAO); // Set using Racket Gray Cube
    // // glBindVertexArray(activeVAO);
    mat4 leftBase = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScale) *
                    scale(mat4(1.0), scalar) * translate(mat4(1.0), vec3(1.05f, BASE_SIDES_HEIGHT, 0.0f)) *
                    rotate(mat4(1.0), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                    scale(mat4(1.0), BASE_SIDES_SCALED);
    setWorldMatrix(shaderProgram, leftBase);
    glDrawArrays(polygons, 0, 36);
    glDrawElements(polygons, 6, GL_UNSIGNED_INT, 0);
    // glDrawElements(polygons, activeVertices, GL_UNSIGNED_INT, 0);

    // right base
    mat4 rightBase = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScale) *
                     scale(mat4(1.0), scalar) * translate(mat4(1.0), vec3(1.95f, BASE_SIDES_HEIGHT, 0.0f)) *
                     rotate(mat4(1.0), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                     scale(mat4(1.0), BASE_SIDES_SCALED);
    setWorldMatrix(shaderProgram, rightBase);
    glDrawArrays(polygons, 0, 36);
    glDrawElements(polygons, 6, GL_UNSIGNED_INT, 0);
    // glDrawElements(polygons, activeVertices, GL_UNSIGNED_INT, 0);

    // Racket Handle --------------------------------------------------
    glBindVertexArray(racketRedAO); // Set using Racket Red Cube
    // // glBindVertexArray(activeVAO);
    mat4 handleRacket = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScale) *
                        scale(mat4(1.0), scalar) *
                        translate(mat4(1.0), vec3(1.5f, 3.75f, 0.0f)) *
                        rotate(mat4(1.0), radians(0.0f), vec3(0.0f, 0.0f, 1.0f)) *
                        scale(mat4(1.0), vec3(0.15f, 2.0f, 0.15f));
    setWorldMatrix(shaderProgram, handleRacket);
    glDrawArrays(polygons, 0, 36);
    glDrawElements(polygons, 6, GL_UNSIGNED_INT, 0);
    glDrawElements(polygons, activeVertices, GL_UNSIGNED_INT, 0);

    draw_arm(shaderProgram, worldMatrixLocation, worldTranslation, degrees, worldRotation, worldScale, polygons, activeVAO, activeVertices);
}
//*************************************************************************************************************
// DRAWING the ARM ---------------------------------------------
void draw_arm(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, float degrees, vec3 worldRotation, vec3 worldScale, float polygons, GLuint activeVAO, int activeVertices)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);
    // glBindVertexArray(activeVAO);

    // Create a scalar vector as base reference
    vec3 scalar = vec3(0.2f, 0.2f, 0.2f);

    int armAO = createTexturedCubeVertexArrayObject(); // Beige Cube for Arm
    glBufferData(GL_ARRAY_BUFFER, sizeof(armCube), armCube, GL_STATIC_DRAW);

    glBindVertexArray(armAO); // Set using Beige cube
    vec3 ARM_SCALED = vec3(0.25f, 1.5f, 0.25f);

    // Lower Arm
    mat4 lowerArmMatrix = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScale) *
                          scale(mat4(1.0), scalar) *
                          translate(mat4(1.0), vec3(1.0f, 1.0f, 0.0f)) *
                          rotate(mat4(1.0), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f)) *
                          scale(mat4(1.0), ARM_SCALED);
    setWorldMatrix(shaderProgram, lowerArmMatrix);
    glDrawArrays(polygons, 0, 36);
    glDrawElements(polygons, 6, GL_UNSIGNED_INT, 0);
    // glDrawElements(polygons, activeVertices, GL_UNSIGNED_INT, 0);

    // Upper Arm
    mat4 upperArmMatrix = translate(mat4(1.0), worldTranslation) * rotate(mat4(1.0), radians(degrees), worldRotation) * scale(mat4(1.0), worldScale) *
                          scale(mat4(1.0), scalar) *
                          translate(mat4(1.0), vec3(1.5f, 2.25f, 0.0f)) *
                          rotate(mat4(1.0), radians(0.0f), vec3(0.0f, 0.0f, 1.0f)) *
                          scale(mat4(1.0), ARM_SCALED);
    setWorldMatrix(shaderProgram, upperArmMatrix);
    glDrawArrays(polygons, 0, 36);
    glDrawElements(polygons, activeVertices, GL_UNSIGNED_INT, 0);
}
//*************************************************************************************************************
// Drawing the tennis ball --------------------------------------------------------
void draw_ball(int shaderProgram, GLuint worldMatrixLocation, vec3 worldTranslation, float polygons, GLuint activeVAO, int activeVertices)
{
    // Compile and link shaders
    glUseProgram(shaderProgram);
    // glBindVertexArray(activeVAO);

    // int sphereAO = createTextured_Sphere_VertexArrayObject();
    // glBufferData(GL_ARRAY_BUFFER, sizeof(sphere), sphere, GL_STATIC_DRAW);

    int sphereAO = createTexturedCubeVertexArrayObject();
    glBufferData(GL_ARRAY_BUFFER, sizeof(greenCube), greenCube, GL_STATIC_DRAW);

    glBindVertexArray(sphereAO);
    
    mat4 ballMatrix = translate(mat4(1.0), worldTranslation) *
                      translate(mat4(1.0), vec3(0.0f, 3.0f, 0.0f)) *
                      scale(mat4(1.0), vec3(0.2f, 0.2f, 0.2f));
    setWorldMatrix(shaderProgram, ballMatrix);
    // glDrawArrays(polygons, 0, (sizeof(sphere) / 3));
    glDrawArrays(polygons, 0, 36);
    glDrawElements(polygons, 100, GL_UNSIGNED_INT, 0);
    glDrawElements(polygons, activeVertices, GL_UNSIGNED_INT, 0);
}

#endif /* MODELOBJECTS_H */
