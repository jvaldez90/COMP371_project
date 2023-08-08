/**
 * @file vertexData.h
 *
 * CONCORDIA UNIVERSITY
 * COMP 371: COMPUTER GRAPHICS
 * Section CC
 * Professor: Kaustubha Mendhurwar
 *
 * TEAM PROJECT: ASSIGNMENT 2
 * DUE DATE: Friday, July 28, 2023
 *
 * @author Joy Anne Valdez
 * Student ID: 2339379
 *
 * @date 2023-07-21
 *
 */

#ifndef VERTEXDATA_H
#define VERTEXDATA_H

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

struct TexturedColoredVertex
{
    TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv)
        : position(_position), color(_color), uv(_uv) {}

    vec3 position;
    vec3 color;
    vec2 uv;
};

// DATA INPUTS FOR GEOMETRIC SHAPES
vec3 colors[] = {
    // BASIC CUBE COLORS
    vec3(1.0f, 1.0f, 0.0f), // [0] YELLOW Cube
    vec3(1.0f, 1.0f, 1.0f), // [1] WHITE Cube
    vec3(1.0f, 0.0f, 0.0f), // [2] RED Cube
    vec3(0.0f, 1.0f, 0.0f), // [3] GREEN Cube
    vec3(0.0f, 0.0f, 1.0f), // [4] BLUE Cube
    vec3(0.0f, 0.0f, 0.0f), // [5] BLACK Cube

    // RACKET COLORS
    vec3(0.6f, 0.0f, 0.0f),    // [6] DARK RED Cube
    vec3(0.75f, 0.75f, 0.75f), // [7] GRAY Cube
    vec3(0.8f, 0.7f, 0.5f),    // [8] BEIGE Cube

    // TENNIS COURT COLORS
    vec3(0.49f, 0.61f, 0.44f), // [9] DARK GREEN Cube
    vec3(0.62f, 0.35f, 0.31f)  // [10] BROWN Cube

};
// Vertices for a 3D Cube
vec3 vertexCoord[] = {
    vec3(-0.5f, -0.5f, -0.5f), // [0] Bottom far left
    vec3(-0.5f, -0.5f, 0.5f),  // [1] Bottom near left
    vec3(0.5f, -0.5f, 0.5f),   // [2] Bottom near right
    vec3(0.5f, -0.5f, -0.5f),  // [3] Bottom far right

    vec3(-0.5f, 0.5f, -0.5f), // [4] Top far left
    vec3(-0.5f, 0.5f, 0.5f),  // [5] Top near left
    vec3(0.5f, 0.5f, 0.5f),   // [6] Top near right
    vec3(0.5f, 0.5f, -0.5f)   // [7] Top far right
};
// Texture Coordinates for mapping textures
vec2 textureCoord[] = {
    vec2(0.0f, 0.0f), // [0] bottom left
    vec2(0.0f, 1.0f), // [1] bottom right
    vec2(1.0f, 1.0f), // [2] top right
    vec2(1.0f, 0.0f)  // [3] top eft
};
vec3 normal[] = {
    vec3(1.0f, 0.0f, 0.0f),  // [0] Right
    vec3(-1.0f, 0.0f, 0.0f), // [1] Left
    vec3(0.0f, 1.0f, 0.0f),  // [2] Top
    vec3(0.0f, -1.0f, 0.0f), // [3] Bottom
    vec3(0.0f, 0.0f, 1.0f),  // [4] Front
    vec3(0.0f, 0.0f, -1.0f)  // [5] Back
};

// WHITE CUBE ------------------------------------------
const TexturedColoredVertex whiteCube[] = {
    //                          position,     color,   texture coord
    TexturedColoredVertex(vertexCoord[0], colors[1], textureCoord[0]), // left
    TexturedColoredVertex(vertexCoord[1], colors[1], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[5], colors[1], textureCoord[2]),

    TexturedColoredVertex(vertexCoord[0], colors[1], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[1], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[1], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[7], colors[1], textureCoord[2]), // far
    TexturedColoredVertex(vertexCoord[0], colors[1], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[4], colors[1], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[7], colors[1], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[3], colors[1], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[0], colors[1], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[2], colors[1], textureCoord[2]), // bottom
    TexturedColoredVertex(vertexCoord[0], colors[1], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[3], colors[1], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[2], colors[1], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[1], colors[1], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[0], colors[1], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[5], colors[1], textureCoord[1]), // near
    TexturedColoredVertex(vertexCoord[1], colors[1], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[2], colors[1], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[1], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[5], colors[1], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[2], colors[1], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[1], textureCoord[2]), // right
    TexturedColoredVertex(vertexCoord[3], colors[1], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[7], colors[1], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[3], colors[1], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[6], colors[1], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[2], colors[1], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[6], colors[1], textureCoord[2]), // top
    TexturedColoredVertex(vertexCoord[7], colors[1], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[4], colors[1], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[6], colors[1], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[1], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[1], textureCoord[1])};

// RED CUBE -------------------------------------------
const TexturedColoredVertex redCube[] = {
    //                          position,     color,   texture coord
    TexturedColoredVertex(vertexCoord[0], colors[2], textureCoord[0]), // left
    TexturedColoredVertex(vertexCoord[1], colors[2], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[5], colors[2], textureCoord[2]),

    TexturedColoredVertex(vertexCoord[0], colors[2], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[2], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[2], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[7], colors[2], textureCoord[2]), // far
    TexturedColoredVertex(vertexCoord[0], colors[2], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[4], colors[2], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[7], colors[2], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[3], colors[2], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[0], colors[2], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[2], colors[2], textureCoord[2]), // bottom
    TexturedColoredVertex(vertexCoord[0], colors[2], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[3], colors[2], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[2], colors[2], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[1], colors[2], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[0], colors[2], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[5], colors[2], textureCoord[1]), // near
    TexturedColoredVertex(vertexCoord[1], colors[2], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[2], colors[2], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[2], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[5], colors[2], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[2], colors[2], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[2], textureCoord[2]), // right
    TexturedColoredVertex(vertexCoord[3], colors[2], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[7], colors[2], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[3], colors[2], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[6], colors[2], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[2], colors[2], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[6], colors[2], textureCoord[2]), // top
    TexturedColoredVertex(vertexCoord[7], colors[2], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[4], colors[2], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[6], colors[2], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[2], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[2], textureCoord[1])};

// GREEN CUBE ---------------------------------------
const TexturedColoredVertex greenCube[] = {
    //                          position,     color,   texture coord
    TexturedColoredVertex(vertexCoord[0], colors[3], textureCoord[0]), // left
    TexturedColoredVertex(vertexCoord[1], colors[3], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[5], colors[3], textureCoord[2]),

    TexturedColoredVertex(vertexCoord[0], colors[3], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[3], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[3], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[7], colors[3], textureCoord[2]), // far
    TexturedColoredVertex(vertexCoord[0], colors[3], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[4], colors[3], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[7], colors[3], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[3], colors[3], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[0], colors[3], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[2], colors[3], textureCoord[2]), // bottom
    TexturedColoredVertex(vertexCoord[0], colors[3], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[3], colors[3], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[2], colors[3], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[1], colors[3], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[0], colors[3], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[5], colors[3], textureCoord[1]), // near
    TexturedColoredVertex(vertexCoord[1], colors[3], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[2], colors[3], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[3], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[5], colors[3], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[2], colors[3], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[3], textureCoord[2]), // right
    TexturedColoredVertex(vertexCoord[3], colors[3], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[7], colors[3], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[3], colors[3], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[6], colors[3], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[2], colors[3], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[6], colors[3], textureCoord[2]), // top
    TexturedColoredVertex(vertexCoord[7], colors[3], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[4], colors[3], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[6], colors[3], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[3], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[3], textureCoord[1])};

// BLUE CUBE ------------------------------------------
const TexturedColoredVertex blueCube[] = {
    //                          position,     color,   texture coord
    TexturedColoredVertex(vertexCoord[0], colors[4], textureCoord[0]), // left
    TexturedColoredVertex(vertexCoord[1], colors[4], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[5], colors[4], textureCoord[2]),

    TexturedColoredVertex(vertexCoord[0], colors[4], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[4], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[4], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[7], colors[4], textureCoord[2]), // far
    TexturedColoredVertex(vertexCoord[0], colors[4], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[4], colors[4], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[7], colors[4], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[3], colors[4], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[0], colors[4], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[2], colors[4], textureCoord[2]), // bottom
    TexturedColoredVertex(vertexCoord[0], colors[4], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[3], colors[4], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[2], colors[4], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[1], colors[4], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[0], colors[4], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[5], colors[4], textureCoord[1]), // near
    TexturedColoredVertex(vertexCoord[1], colors[4], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[2], colors[4], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[4], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[5], colors[4], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[2], colors[4], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[4], textureCoord[2]), // right
    TexturedColoredVertex(vertexCoord[3], colors[4], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[7], colors[4], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[3], colors[4], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[6], colors[4], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[2], colors[4], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[6], colors[4], textureCoord[2]), // top
    TexturedColoredVertex(vertexCoord[7], colors[4], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[4], colors[4], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[6], colors[4], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[4], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[4], textureCoord[1])};

// BLACK CUBE ------------------------------------------
const TexturedColoredVertex blackCube[] = {
    //                          position,     color,   texture coord
    TexturedColoredVertex(vertexCoord[0], colors[5], textureCoord[0]), // left
    TexturedColoredVertex(vertexCoord[1], colors[5], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[5], colors[5], textureCoord[2]),

    TexturedColoredVertex(vertexCoord[0], colors[5], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[5], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[5], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[7], colors[5], textureCoord[2]), // far
    TexturedColoredVertex(vertexCoord[0], colors[5], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[4], colors[5], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[7], colors[5], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[3], colors[5], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[0], colors[5], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[2], colors[5], textureCoord[2]), // bottom
    TexturedColoredVertex(vertexCoord[0], colors[5], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[3], colors[5], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[2], colors[5], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[1], colors[5], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[0], colors[5], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[5], colors[5], textureCoord[1]), // near
    TexturedColoredVertex(vertexCoord[1], colors[5], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[2], colors[5], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[5], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[5], colors[5], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[2], colors[5], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[5], textureCoord[2]), // right
    TexturedColoredVertex(vertexCoord[3], colors[5], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[7], colors[5], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[3], colors[5], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[6], colors[5], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[2], colors[5], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[6], colors[5], textureCoord[2]), // top
    TexturedColoredVertex(vertexCoord[7], colors[5], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[4], colors[5], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[6], colors[5], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[5], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[5], textureCoord[1])};
// RACKET RED CUBE ----------------------------------
const TexturedColoredVertex racketRed[] = {
    //                          position,     color,   texture coord
    TexturedColoredVertex(vertexCoord[0], colors[6], textureCoord[0]), // left
    TexturedColoredVertex(vertexCoord[1], colors[6], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[5], colors[6], textureCoord[2]),

    TexturedColoredVertex(vertexCoord[0], colors[6], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[6], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[6], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[7], colors[6], textureCoord[2]), // far
    TexturedColoredVertex(vertexCoord[0], colors[6], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[4], colors[6], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[7], colors[6], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[3], colors[6], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[0], colors[6], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[2], colors[6], textureCoord[2]), // bottom
    TexturedColoredVertex(vertexCoord[0], colors[6], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[3], colors[6], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[2], colors[6], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[1], colors[6], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[0], colors[6], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[5], colors[6], textureCoord[1]), // near
    TexturedColoredVertex(vertexCoord[1], colors[6], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[2], colors[6], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[6], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[5], colors[6], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[2], colors[6], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[6], textureCoord[2]), // right
    TexturedColoredVertex(vertexCoord[3], colors[6], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[7], colors[6], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[3], colors[6], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[6], colors[6], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[2], colors[6], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[6], colors[6], textureCoord[2]), // top
    TexturedColoredVertex(vertexCoord[7], colors[6], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[4], colors[6], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[6], colors[6], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[6], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[6], textureCoord[1])};

// RACKET GRAY CUBE ------------------------------------
const TexturedColoredVertex racketGray[] = {
    //                          position,     color,   texture coord
    TexturedColoredVertex(vertexCoord[0], colors[7], textureCoord[0]), // left
    TexturedColoredVertex(vertexCoord[1], colors[7], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[5], colors[7], textureCoord[2]),

    TexturedColoredVertex(vertexCoord[0], colors[7], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[7], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[7], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[7], colors[7], textureCoord[2]), // far
    TexturedColoredVertex(vertexCoord[0], colors[7], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[4], colors[7], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[7], colors[7], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[3], colors[7], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[0], colors[7], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[2], colors[7], textureCoord[2]), // bottom
    TexturedColoredVertex(vertexCoord[0], colors[7], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[3], colors[7], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[2], colors[7], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[1], colors[7], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[0], colors[7], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[5], colors[7], textureCoord[1]), // near
    TexturedColoredVertex(vertexCoord[1], colors[7], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[2], colors[7], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[7], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[5], colors[7], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[2], colors[7], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[7], textureCoord[2]), // right
    TexturedColoredVertex(vertexCoord[3], colors[7], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[7], colors[7], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[3], colors[7], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[6], colors[7], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[2], colors[7], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[6], colors[7], textureCoord[2]), // top
    TexturedColoredVertex(vertexCoord[7], colors[7], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[4], colors[7], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[6], colors[7], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[7], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[7], textureCoord[1])};

// ARM CUBE ----------------------------------------------
const TexturedColoredVertex armCube[] = {
    //                          position,     color,   texture coord
    TexturedColoredVertex(vertexCoord[0], colors[8], textureCoord[0]), // left
    TexturedColoredVertex(vertexCoord[1], colors[8], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[5], colors[8], textureCoord[2]),

    TexturedColoredVertex(vertexCoord[0], colors[8], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[8], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[8], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[7], colors[8], textureCoord[2]), // far
    TexturedColoredVertex(vertexCoord[0], colors[8], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[4], colors[8], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[7], colors[8], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[3], colors[8], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[0], colors[8], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[2], colors[8], textureCoord[2]), // bottom
    TexturedColoredVertex(vertexCoord[0], colors[8], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[3], colors[8], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[2], colors[8], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[1], colors[8], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[0], colors[8], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[5], colors[8], textureCoord[1]), // near
    TexturedColoredVertex(vertexCoord[1], colors[8], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[2], colors[8], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[8], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[5], colors[8], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[2], colors[8], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[8], textureCoord[2]), // right
    TexturedColoredVertex(vertexCoord[3], colors[8], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[7], colors[8], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[3], colors[8], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[6], colors[8], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[2], colors[8], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[6], colors[8], textureCoord[2]), // top
    TexturedColoredVertex(vertexCoord[7], colors[8], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[4], colors[8], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[6], colors[8], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[8], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[8], textureCoord[1])};

const TexturedColoredVertex courtGround[] = {
    //                          position,     color,   texture coord
    TexturedColoredVertex(vertexCoord[0], colors[9], textureCoord[0]), // left
    TexturedColoredVertex(vertexCoord[1], colors[9], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[5], colors[9], textureCoord[2]),

    TexturedColoredVertex(vertexCoord[0], colors[9], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[9], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[9], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[7], colors[9], textureCoord[2]), // far
    TexturedColoredVertex(vertexCoord[0], colors[9], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[4], colors[9], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[7], colors[9], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[3], colors[9], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[0], colors[9], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[2], colors[9], textureCoord[2]), // bottom
    TexturedColoredVertex(vertexCoord[0], colors[9], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[3], colors[9], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[2], colors[9], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[1], colors[9], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[0], colors[9], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[5], colors[9], textureCoord[1]), // near
    TexturedColoredVertex(vertexCoord[1], colors[9], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[2], colors[9], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[9], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[5], colors[9], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[2], colors[9], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[9], textureCoord[2]), // right
    TexturedColoredVertex(vertexCoord[3], colors[9], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[7], colors[9], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[3], colors[9], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[6], colors[9], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[2], colors[9], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[6], colors[9], textureCoord[2]), // top
    TexturedColoredVertex(vertexCoord[7], colors[9], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[4], colors[9], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[6], colors[9], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[9], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[9], textureCoord[1])};

const TexturedColoredVertex brownCube[] = {
    //                          position,     color,   texture coord
    TexturedColoredVertex(vertexCoord[0], colors[10], textureCoord[0]), // left
    TexturedColoredVertex(vertexCoord[1], colors[10], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[5], colors[10], textureCoord[2]),

    TexturedColoredVertex(vertexCoord[0], colors[10], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[10], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[10], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[7], colors[10], textureCoord[2]), // far
    TexturedColoredVertex(vertexCoord[0], colors[10], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[4], colors[10], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[7], colors[10], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[3], colors[10], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[0], colors[10], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[2], colors[10], textureCoord[2]), // bottom
    TexturedColoredVertex(vertexCoord[0], colors[10], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[3], colors[10], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[2], colors[10], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[1], colors[10], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[0], colors[10], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[5], colors[10], textureCoord[1]), // near
    TexturedColoredVertex(vertexCoord[1], colors[10], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[2], colors[10], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[10], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[5], colors[10], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[2], colors[10], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[10], textureCoord[2]), // right
    TexturedColoredVertex(vertexCoord[3], colors[10], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[7], colors[10], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[3], colors[10], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[6], colors[10], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[2], colors[10], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[6], colors[10], textureCoord[2]), // top
    TexturedColoredVertex(vertexCoord[7], colors[10], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[4], colors[10], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[6], colors[10], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[10], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[10], textureCoord[1])};

// Textured Cube model - Default Yellow cube
const TexturedColoredVertex texturedCubeVertexArray[] = {
    //                          position,     color,   texture coord
    TexturedColoredVertex(vertexCoord[0], colors[0], textureCoord[0]), // left
    TexturedColoredVertex(vertexCoord[1], colors[0], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[5], colors[0], textureCoord[2]),

    TexturedColoredVertex(vertexCoord[0], colors[0], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[0], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[0], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[7], colors[0], textureCoord[2]), // far
    TexturedColoredVertex(vertexCoord[0], colors[0], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[4], colors[0], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[7], colors[0], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[3], colors[0], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[0], colors[0], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[2], colors[0], textureCoord[2]), // bottom
    TexturedColoredVertex(vertexCoord[0], colors[0], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[3], colors[0], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[2], colors[0], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[1], colors[0], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[0], colors[0], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[5], colors[0], textureCoord[1]), // near
    TexturedColoredVertex(vertexCoord[1], colors[0], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[2], colors[0], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[0], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[5], colors[0], textureCoord[1]),
    TexturedColoredVertex(vertexCoord[2], colors[0], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[6], colors[0], textureCoord[2]), // right
    TexturedColoredVertex(vertexCoord[3], colors[0], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[7], colors[0], textureCoord[3]),

    TexturedColoredVertex(vertexCoord[3], colors[0], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[6], colors[0], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[2], colors[0], textureCoord[1]),

    TexturedColoredVertex(vertexCoord[6], colors[0], textureCoord[2]), // top
    TexturedColoredVertex(vertexCoord[7], colors[0], textureCoord[3]),
    TexturedColoredVertex(vertexCoord[4], colors[0], textureCoord[0]),

    TexturedColoredVertex(vertexCoord[6], colors[0], textureCoord[2]),
    TexturedColoredVertex(vertexCoord[4], colors[0], textureCoord[0]),
    TexturedColoredVertex(vertexCoord[5], colors[0], textureCoord[1])};

// Sphere object

#endif /* VERTEXDATA_H */