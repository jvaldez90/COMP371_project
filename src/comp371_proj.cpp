/*
 * @file comp371_proj.cpp
 *
 * CONCORDIA UNIVERSITY
 * COMP 371: COMPUTER GRAPHICS
 * Section CC
 * Professor: Kaustubha Mendhurwar
 *
 * FINAL PROJECT
 * DUE DATE: Friday, August 11, 2023
 *
 * @author Joy Anne Valdez
 * Student ID: 26339379
 *
 * @date 2023-07-21
 *
 */

#include <glm/glm.hpp>
#include <cstring>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <sstream>

#define GLEW_STATIC 1
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include "../include/comp371.h"
#include "../include/modelObjects.h"
#include "../include/OBJloader.h"   //For loading .obj files
#include "../include/OBJloaderV2.h" //For loading .obj files using a polygon list format

using namespace glm;
using namespace std;

GLuint setupModelVBO(string path, int &vertexCount);

// Sets up a model using an Element Buffer Object to refer to vertex data
GLuint setupModelEBO(string path, int &vertexCount);

// shader variable setters
void SetUniformMat4(GLuint shader_id, const char *uniform_name, mat4 uniform_value)
{
    glUseProgram(shader_id);
    glUniformMatrix4fv(glGetUniformLocation(shader_id, uniform_name), 1, GL_FALSE, &uniform_value[0][0]);
}

void SetUniformVec3(GLuint shader_id, const char *uniform_name, vec3 uniform_value)
{
    glUseProgram(shader_id);
    glUniform3fv(glGetUniformLocation(shader_id, uniform_name), 1, value_ptr(uniform_value));
}

template <class T>
void SetUniform1Value(GLuint shader_id, const char *uniform_name, T uniform_value)
{
    glUseProgram(shader_id);
    glUniform1i(glGetUniformLocation(shader_id, uniform_name), uniform_value);
    glUseProgram(0);
}
GLFWwindow *window = nullptr;
bool InitContext();

int main(int argc, char *argv[])
{
    // REFERNCE [LAB 4]
    // Initialize GLFW and OpenGL version
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow *window = glfwCreateWindow(1024, 768, "COMP 371 Summer 2023 - Team Final Project", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set Background Color
    float r = 0.6196078431f;
    float g = 0.8f;
    float b = 0.9098039216f;
    glClearColor(r, g, b, 1.0f);

    // REFERENCES from [LAB 8]
    std::string shaderPathPrefix = "assets/shaders/";
    GLuint shaderScene = loadSHADER(shaderPathPrefix + "scene_vertex.glsl", shaderPathPrefix + "scene_fragment.glsl");
    GLuint shaderShadow = loadSHADER(shaderPathPrefix + "shadow_vertex.glsl", shaderPathPrefix + "shadow_fragment.glsl");

    // REFERENCE from [LAB 4]
    // Compile and link shaders
    int colorShaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
    int texturedShaderProgram = compileAndLinkShaders(getTexturedVertexShaderSource(), getTexturedFragmentShaderSource());

    // Load Textures
    GLuint ballTextureID = loadTexture("assets/textures/ball.jpg");
    GLuint clayTextureID = loadTexture("assets/textures/clay.jpg");
    GLuint courtTextureID = loadTexture("assets/textures/court.jpg");
    GLuint glossTextureID = loadTexture("assets/textures/gloss.jpg");

    // Setup models
    string cubePath = "assets/models/cube.obj";
    int cubeVertices;
    GLuint cubeVAO = setupModelEBO(cubePath, cubeVertices);

    int activeVertices = cubeVertices;
    GLuint activeVAO = cubeVAO;

    // Setup texture and framebuffer for creating shadow map
    // Dimensions of the shadow texture, which should cover the viewport window size and shouldn't be oversized and waste resources
    const unsigned int DEPTH_MAP_TEXTURE_SIZE = 1024;

    // Variable storing index to texture used for shadow mapping
    GLuint depth_map_texture;
    // Get the texture
    glGenTextures(1, &depth_map_texture);
    // Bind the texture so the next glTex calls affect it
    glBindTexture(GL_TEXTURE_2D, depth_map_texture);
    // Create the texture and specify it's attributes, including widthn height, components (only depth is stored, no color information)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 NULL);
    // Set texture sampler parameters.
    // The two calls below tell the texture sampler inside the shader how to upsample and downsample the texture. Here we choose the nearest filtering option, which means we just use the value of the closest pixel to the chosen image coordinate.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // The two calls below tell the texture sampler inside the shader how it should deal with texture coordinates outside of the [0, 1] range. Here we decide to just tile the image.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Variable storing index to framebuffer used for shadow mapping
    GLuint depth_map_fbo; // fbo: framebuffer object
    // Get the framebuffer
    glGenFramebuffers(1, &depth_map_fbo);
    // Bind the framebuffer so the next glFramebuffer calls affect it
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    // Attach the depth map texture to the depth map framebuffer
    // glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depth_map_texture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_texture, 0);
    glDrawBuffer(GL_NONE); // disable rendering colors, only write depth values

    // Global Variables for object transformations
    vec3 worldTranslation = vec3(0.0f, 0.0f, 0.0f);
    float degrees = 0.0f;
    vec3 worldRotation = vec3(0.0f, 1.0f, 0.0f);
    vec3 worldScale = vec3(1.0f, 1.0f, 1.0f);

    int counter = 1;
    bool is_on = false;        // Toggle between on / off for grid and axis origin
    bool is_textureOn = false; // Toggle between on / off for textures
    bool is_shadowOn = false;  // Toggle between on / off for shadows

    // Camera parameters for view transform
    glm::vec3 cameraPosition(0.0f, 2.0f, 5.0f); // EYE
    glm::vec3 cameraLookAt(0.0f, 0.0f, -1.0f);  // AT
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);       // UP

    // Other camera parameters
    float cameraSpeed = 1.0f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    bool cameraFirstPerson = true; // press 1 or 2 to toggle this variable

    // For spinning model
    float spinningAngle = 0.0f;

    // Set projection matrix for shader, this won't change
    glm::mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
                                                  1024.0f / 768.0f, // aspect ratio
                                                  0.01f, 100.0f);   // near and far (near > 0)

    // Set initial view matrix
    glm::mat4 viewMatrix = lookAt(cameraPosition,                // eye
                                  cameraPosition + cameraLookAt, // center
                                  cameraUp);                     // up

    // Set Projection matrices on both shaders
    setProjectionMatrix(colorShaderProgram, projectionMatrix);
    setProjectionMatrix(texturedShaderProgram, projectionMatrix);
    // setProjectionMatrix(shaderScene, projectionMatrix);
    // setProjectionMatrix(shaderShadow, projectionMatrix);
    SetUniformMat4(shaderScene, "projection_matrix", projectionMatrix);

    // Set view matrix on both shaders
    setViewMatrix(colorShaderProgram, viewMatrix);
    setViewMatrix(texturedShaderProgram, viewMatrix);
    // setProjectionMatrix(shaderScene, projectionMatrix);
    // setProjectionMatrix(shaderShadow, projectionMatrix);
    SetUniformMat4(shaderScene, "view_matrix", viewMatrix);

    // Define and upload geometry to the GPU
    int texturedCubeVAO = createTexturedCubeVertexArrayObject();

    // *************************************************
    // REFERENCE CODE from [LAB 8]
    // Set up for light source

    float lightAngleOuter = 30.0;
    float lightAngleInner = 20.0;
    // Set light cutoff angles on scene shader
    SetUniform1Value(shaderScene, "light_cutoff_inner", cos(radians(lightAngleInner)));
    SetUniform1Value(shaderScene, "light_cutoff_outer", cos(radians(lightAngleOuter)));

    // Set light color on scene shader
    SetUniformVec3(shaderScene, "light_color", vec3(1.0, 1.0, 1.0));

    // Set object color on scene shader
    SetUniformVec3(shaderScene, "object_color", vec3(1.0, 1.0, 1.0));

    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
    // ******************************************************************

    // Other OpenGL states to set once
    // Enable Backface culling
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Each frame, reset color of each pixel to glClearColor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //*******************************************
        // light parameters
        vec3 lightPosition =
            // vec3(0.6f, 30.0f, 5.0f); // the location of the light in 3D space
            vec3(sinf(glfwGetTime() * 6.0f * 3.141592f), sinf(glfwGetTime() * 3.141592f), cosf(glfwGetTime() * 3.141592f));
        vec3 lightFocus(0.0, 0.0, -1.0); // the point in 3D space the light "looks" at
        vec3 lightDirection = normalize(lightFocus - lightPosition);

        float lightNearPlane = 1.0f;
        float lightFarPlane = 180.0f;

        mat4 lightProjectionMatrix =
            // frustum(-1.0f, 10.0f, -1.0f, 1.0f, lightNearPlane, lightFarPlane);
            perspective(20.0f, (float)DEPTH_MAP_TEXTURE_SIZE / (float)DEPTH_MAP_TEXTURE_SIZE, lightNearPlane, lightFarPlane);
        mat4 lightViewMatrix = lookAt(lightPosition, lightFocus, vec3(0.0f, 10.0f, 0.0f));
        mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;

        // Set light space matrix on both shaders
        SetUniformMat4(shaderShadow, "light_view_proj_matrix", lightSpaceMatrix);
        SetUniformMat4(shaderScene, "light_view_proj_matrix", lightSpaceMatrix);

        // Set light far and near planes on scene shader
        SetUniform1Value(shaderScene, "light_near_plane", lightNearPlane);
        SetUniform1Value(shaderScene, "light_far_plane", lightFarPlane);

        // Set light position on scene shader
        SetUniformVec3(shaderScene, "light_position", lightPosition);

        // Set light direction on scene shader
        SetUniformVec3(shaderScene, "light_direction", lightDirection);

        // Spinning model rotation animation
        spinningAngle += 45.0f * dt; // This is equivalent to 45 degrees per second

        // Set model matrix and send to both shaders
        mat4 modelMatrix =
            mat4(1.0f);
        // glm::translate(mat4(1.0f), vec3(0.0f, 5.0f, 0.0f))
        // glm::rotate(mat4(1.0f), radians(spinningAngle), vec3(0.0f, 1.0f, 0.0f)) *
        // glm::rotate(mat4(1.0f), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f)) *
        // glm::scale(mat4(1.0f), vec3(1.0f));

        SetUniformMat4(shaderScene, "model_matrix", modelMatrix);
        SetUniformMat4(shaderShadow, "model_matrix", modelMatrix);

        // Set the view matrix for first person camera and send to both shaders
        mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        SetUniformMat4(shaderScene, "view_matrix", viewMatrix);

        // Set view position on scene shader
        SetUniformVec3(shaderScene, "view_position", cameraPosition);

        //*******************************************
        // REFERENCE from [LAB 4]

        // Draw textured geometry
        glUseProgram(texturedShaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ballTextureID);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, clayTextureID);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, courtTextureID);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, glossTextureID);

        GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
        GLuint textureLocation2 = glGetUniformLocation(texturedShaderProgram, "textureSampler2");
        GLuint textureLocation3 = glGetUniformLocation(texturedShaderProgram, "textureSampler3");
        GLuint textureLocation4 = glGetUniformLocation(texturedShaderProgram, "textureSampler4");

        glUniform1i(textureLocation, 0); // Set our Texture sampler to user Texture Unit 0
        glUniform1i(textureLocation2, 1);
        glUniform1i(textureLocation3, 2);
        glUniform1i(textureLocation4, 3);

        // Draw ground
        mat4 groundWorldMatrix = translate(mat4(1.0), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0), vec3(0.5f, 0.5f, 0.5f));
        GLuint worldMatrixLocation = glGetUniformLocation(colorShaderProgram, "worldMatrix");
        setWorldMatrix(colorShaderProgram, groundWorldMatrix);
        setWorldMatrix(texturedShaderProgram, groundWorldMatrix);

        // Draw colored geometry
        glUseProgram(colorShaderProgram);
        // For pressing Key [O] to turn grid on or off
        if (is_on)
        {
            draw_grid(texturedCubeVAO, colorShaderProgram, worldMatrixLocation, vec3(0.0f, 0.02f, 0.0f));
        }
        // Toggle between on/off for textures
        glUseProgram(texturedShaderProgram);
        if (is_textureOn)
        {
            // Assign texturedShaderProgram to a shader Program
            // Then draw the objects onto the scene
            { // World Environment Set up
                glBindTexture(GL_TEXTURE_2D, clayTextureID);
                draw_ground(texturedShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, 0.0f));
                glBindTexture(GL_TEXTURE_2D, ballTextureID);
                draw_ball(texturedShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, 0.0f), polygons, activeVAO, activeVertices);
                glBindTexture(GL_TEXTURE_2D, courtTextureID);
                draw_court(texturedShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, 0.0f));
            }

            glBindTexture(GL_TEXTURE_2D, glossTextureID);
            draw_net(texturedShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, 0.0f), activeVAO, activeVertices);
            draw_V(texturedShaderProgram, worldMatrixLocation, vec3(-3.0f, 2.5f, -1.7f), degrees, worldRotation, worldScale, activeVAO, activeVertices);

            // is_textureOn = false;
        }
        // Toggle between on/off for shadows
        if (is_shadowOn)
        {
            // Render shadow in 2 passes: 1- Render depth map, 2- Render scene
            // 1- Render shadow map:
            // a- use program for shadows
            // b- resize window coordinates to fix depth map output size
            // c- bind depth map framebuffer to output the depth values
            {
                // Use proper shader
                glUseProgram(shaderShadow);
                // Use proper image output size
                glViewport(0, 0, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE);
                // Bind depth map texture as output framebuffer
                glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
                // Clear depth data on the framebuffer
                glClear(GL_DEPTH_BUFFER_BIT);
                // Bind geometry
                glBindVertexArray(activeVAO);
                // Draw geometry
                {
                    draw_net(shaderShadow, worldMatrixLocation, vec3(0.0f, 0.0f, 0.0f), activeVAO, activeVertices);

                    draw_V(shaderShadow, worldMatrixLocation, vec3(-3.0f, 2.5f, -1.7f), degrees, worldRotation, worldScale, activeVAO, activeVertices);

                    // glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);
                }
                // Unbind geometry
                glBindVertexArray(0);
            }
            // 2- Render scene: a- bind the default framebuffer and b- just render like what we do normally
            {
                // Use proper shader
                glUseProgram(shaderScene);
                // Use proper image output size
                // Side note: we get the size from the framebuffer instead of using WIDTH and HEIGHT because of a bug with highDPI displays
                int width, height;
                glfwGetFramebufferSize(window, &width, &height);
                glViewport(0, 0, width, height);
                // Bind screen as output framebuffer
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                // Clear color and depth data on framebuffer
                glClearColor(r, g, b, 1.0f);
                // glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                // Bind depth map texture: not needed, by default it is active
                glActiveTexture(GL_TEXTURE0);
                // Bind geometry
                glBindVertexArray(activeVAO);
                // Draw geometry
                {
                    // draw_light(shaderShadow, worldMatrixLocation, vec3(0.0f, 20.f, 0.0f), activeVAO, activeVertices);
                    draw_net(shaderScene, worldMatrixLocation, vec3(0.0f, 0.0f, 0.0f), activeVAO, activeVertices);
                    draw_ball(shaderScene, worldMatrixLocation, vec3(0.0f, 0.0f, 0.0f), polygons, activeVAO, activeVertices);

                    draw_V(shaderScene, worldMatrixLocation, vec3(-3.0f, 2.5f, -1.7f), degrees, worldRotation, worldScale, activeVAO, activeVertices);

                    // glDrawElements(GL_TRIANGLES, activeVertices, GL_UNSIGNED_INT, 0);
                }

                // Unbind geometry
                glBindVertexArray(0);
            }
            // is_shadowOn = false;
        }

        // DRAW OBJECTS into the scene
        { // Static Objects

            draw_ground(colorShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, 0.0f));
            draw_court(colorShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, 0.0f));
            draw_net(colorShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, 0.0f), activeVAO, activeVertices);
            draw_courtLines(colorShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, 0.0f));
        }

        draw_V(colorShaderProgram, worldMatrixLocation, vec3(-3.0f, 2.5f, -1.7f), degrees, worldRotation, worldScale, activeVAO, activeVertices);

        /* DRAWING AN ENVIRONMENT into scene */
        // Draw an empty court on the in the negative z-plane
        draw_ground(colorShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, -12.0f));
        draw_court(colorShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, -12.0f));
        draw_net(colorShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, -12.0f), activeVAO, activeVertices);
        draw_courtLines(colorShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, -12.0f));

        // Draw another empty court on the in the positive z-plane
        draw_ground(colorShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, 12.0f));
        draw_court(colorShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, 12.0f));
        draw_net(colorShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, 12.0f), activeVAO, activeVertices);
        draw_courtLines(colorShaderProgram, worldMatrixLocation, vec3(0.0f, 0.0f, 12.0f));

        // Spinning cube at camera position
        spinningAngle += 180.0f * dt;

        // Draw avatar in view space for first person camera
        // and in world space for third person camera
        if (cameraFirstPerson)
        {
            // Wolrd matrix is identity, but view transform like a world transform relative to camera basis
            // (1 unit in front of camera)
            //
            // This is similar to a weapon moving with camera in a shooter game
            mat4 spinningCubeViewMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0f)) *
                                          rotate(mat4(1.0f), radians(spinningAngle), vec3(0.0f, 1.0f, 0.0f)) *
                                          scale(mat4(1.0f), vec3(0.01f, 0.01f, 0.01f));

            setWorldMatrix(colorShaderProgram, mat4(1.0f));
            // setViewMatrix(colorShaderProgram, spinningCubeViewMatrix);
        }
        else
        {
            // In third person view, let's draw the spinning cube in world space, like any other models
            mat4 spinningCubeWorldMatrix = translate(mat4(1.0f), cameraPosition) *
                                           rotate(mat4(1.0f), radians(spinningAngle), vec3(0.0f, 1.0f, 0.0f)) *
                                           scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));
        }
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        // END FRAME
        glfwSwapBuffers(window);
        // Detect inputs
        glfwPollEvents();
        /**********************************************************************************************************/
        /******************************************   END SCENE   *************************************************/
        /**********************************************************************************************************/

        //****  HANDLE INPUTS ****//--------------------------------
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = true;
        }
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = false;
        }
        // A first or third person camera
        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;

        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;

        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        // Convert to spherical coordinates
        const float cameraAngularSpeed = 60.0f;
        cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

        // Reference: Interactive computer graphics: a top-down approach with shader-based OpenGL
        //                (6th edition) - Edward Angel, Dave Shreiner: 2.11.4 The Idle Callback (p. 104)
        const float degreesToRadians = 3.14f / 180.0f;
        float angle = 5.0f * degreesToRadians;

        // Arbitrary variables to change x, y, z coordinates
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        // Clamp vertical angle to [-85, 85] degrees
        cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
        if (cameraHorizontalAngle > 360)
        {
            cameraHorizontalAngle -= 360;
        }
        else if (cameraHorizontalAngle < -360)
        {
            cameraHorizontalAngle += 360;
        }

        float theta = radians(cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);

        cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

        glm::normalize(cameraSideVector);
        /***************************
         *  OTHER KEYBOARD INPUTS  *
         ***************************/
        // When Space Bar is pressed ----------------------------------------------
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        { // If pressed multiple times change the position of the camera direction
            if (counter == 1)
            {                                              // Front Left Side View of Object Space
                cameraPosition = vec3(10.0f, 3.0f, 10.0f); // EYE
                cameraLookAt = vec3(-10.0f, 3.0f, -10.0f); // AT
                cameraUp = vec3(0.0f, 1.0f, 0.0f);         // UP

                counter++;
            }
            else if (counter == 2)
            {                                               // Front Right Side View of Object Space
                cameraPosition = vec3(-10.0f, 3.0f, 10.0f); // EYE
                cameraLookAt = vec3(10.0f, 3.0f, -10.0f);   // AT
                cameraUp = vec3(0.0f, 1.0f, 0.0f);          // UP

                counter++;
            }
            else if (counter == 3)
            {                                               // Back Left Side View of Object Space
                cameraPosition = vec3(10.0f, 3.0f, -10.0f); // EYE
                cameraLookAt = vec3(-10.0f, 3.0f, 10.0f);   // AT
                cameraUp = vec3(0.0f, 1.0f, 0.0f);          // UP

                counter++;
            }
            else if (counter == 4)
            {                                                // Back Right Side VIew of Object Space
                cameraPosition = vec3(-10.0f, 3.0f, -10.0f); // EYE
                cameraLookAt = vec3(10.0f, 3.0f, 10.0f);     // AT
                cameraUp = vec3(0.0f, 1.0f, 0.0f);           // UP

                counter++;
            }
            else if (counter == 5)
            { // If counter == 5 reset it back to 1
                counter = 1;
                cameraPosition = vec3(1.0f, 3.0f, 10.0f); // EYE
                cameraLookAt = vec3(0.0f, 3.0f, -10.0f);  // AT
                cameraUp = vec3(0.0f, 1.0f, 0.0f);        // UP
            }
        }
        /****************************
         *  LETTER KEYBOARD INPUTS  *
         ****************************/
        // When [U J] keys are pressed ------------------------------
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // Suppose to Scale-Up the model
        {
            worldScale /= 0.99;
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) // Suppose to Scale_Down the model
        {
            worldScale *= 0.99;
        }
        // When [P L T] Keys are pressed
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        { // Sets to draw vertices as points
            polygons = GL_POINTS;
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        { // Sets to draw vertices as lines
            polygons = GL_LINES;
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        { // sets to draw vertices as triangles
            polygons = GL_TRIANGLES;
        }
        // When [O] Key is pressed
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        { // Toogle between grid and axis origin on / off
            if (is_on)
                is_on = false;
            else
                is_on = true;
        }
        // When [X] Key is pressed
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        { // Toogle betweeen textures on / off
            if (is_textureOn)
                is_textureOn = false;
            else
                is_textureOn = true;
        }
        // When [B] Key is pressed
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        { // Toogle between shadows on / off
            if (is_shadowOn)
                is_shadowOn = false;
            else
                is_shadowOn = true;
        }

        // Reference: Interactive computer graphics: A Top-Down Approach With Shader-Based OpenGL
        //            (6th edition) - Edward Angel, Dave Shreiner (2012): 2.11.4 The Idle Callback (p. 104)

        /*************************
         *  ARROW BUTTON INPUTS  *
         *************************/
        // When UP and DOWN Arrow keys are pressed ----------------------------
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // Suppose to be Rx
        {
            y = cosf(angle) * cameraPosition.y - sinf(angle) * cameraPosition.z;
            z = sinf(angle) * cameraPosition.y + cosf(angle) * cameraPosition.z;
            cameraPosition.y = y;
            cameraPosition.z = z;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // Suppose to be R-x
        {
            angle = -5.0f * degreesToRadians;
            y = cosf(angle) * cameraPosition.y - sinf(angle) * cameraPosition.z;
            z = sinf(angle) * cameraPosition.y + cosf(angle) * cameraPosition.z;
            cameraPosition.y = y;
            cameraPosition.z = z;
        }
        // When LEFT and RIGHT Arrows keys are pressed -------------------------
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // Suppose to be Ry
        {
            x = cosf(angle) * cameraPosition.x + sinf(angle) * cameraPosition.z;
            z = -sinf(angle) * cameraPosition.x + cosf(angle) * cameraPosition.z;
            cameraPosition.x = x;
            cameraPosition.z = z;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // Suppose to be R-y
        {
            angle = -5.0f * degreesToRadians;
            x = cosf(angle) * cameraPosition.x + sinf(angle) * cameraPosition.z;
            z = -sinf(angle) * cameraPosition.x + cosf(angle) * cameraPosition.z;
            cameraPosition.x = x;
            cameraPosition.z = z;
        }

        // When [w a s d] keys are pressed ----------------------------
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        { // Move camera FORWARD along the Z-Axis
            cameraPosition += cameraLookAt * dt * currentCameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        { // Move camera BACKWARD along the Z-Axis
            cameraPosition -= cameraLookAt * dt * currentCameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // pan camera to the right
        {
            cameraPosition += cameraSideVector * dt * currentCameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPosition -= cameraSideVector * dt * currentCameraSpeed;
        }

        // When LEFT SHIFT + [W A S D] Key is press ------------------------------------
        if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)) // pan camera up
        {
            cameraPosition.y += currentCameraSpeed * dt * currentCameraSpeed;
        }
        if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)) // pan camers down
        {
            cameraPosition.y -= currentCameraSpeed * dt * currentCameraSpeed;
        }
        if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS))
        {
            cameraPosition -= cameraSideVector * dt * currentCameraSpeed; // pan camera to the left
        }
        if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS))
        {
            cameraPosition += cameraSideVector * dt * currentCameraSpeed; // pan camera to the right
        }
        // When [1 2 3 4] keys are pressed
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            // Selecting V character
            cameraPosition = vec3(-3.0f, 2.5f, -1.7f); // EYE
            cameraLookAt = vec3(3.0f, 2.0f, 1.7);      // AT
            cameraUp = vec3(0.0f, 1.0f, 0.0f);         // UP
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            // Selecting A character
            cameraPosition = vec3(-3.0f, 2.5f, 1.7f); // EYE
            cameraLookAt = vec3(3.0f, 2.0f, 1.7f);    // AT
            cameraUp = vec3(0.0f, 1.0f, 0.0f);        // UP
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            // Selecting L character
            cameraPosition = vec3(3.0f, 2.5f, -1.7f); // EYE
            cameraLookAt = vec3(-3.0f, 2.0f, -1.7f);  // AT
            cameraUp = vec3(0.0f, 1.0f, 0.0f);        // UP
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            // Selecting D character
            cameraPosition = vec3(3.0f, 2.5f, 1.7f); // EYE
            cameraLookAt = vec3(-3.0f, 2.0f, 1.7f);  // AT
            cameraUp = vec3(0.0f, 1.0f, 0.0f);       // UP
        }
        /***********************************************************************
         * COMMA | PERIOD | SEMICOLON | SLASH || C | V BUTTONS || ENTER BUTTON *
         ***********************************************************************/
        if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
        {
            // POSTIVE ROTATATION of object along the Z-AXIS
            worldRotation = vec3(worldRotation.x, worldRotation.y, 1.0f);
            if (degrees >= 90)
                degrees = 90;
            else
                degrees += 1;
        }
        if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
        {
            // NEGATIVE ROTATION of object along the Z-AXIS
            worldRotation = vec3(worldRotation.x, worldRotation.y, 1.0f);
            if (degrees <= -90)
                degrees = -90;
            else
                degrees -= 1;
        }
        if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS)
        {
            // POSITIVE ROTATION of object along the X-AXIS
            worldRotation = vec3(1.0f, worldRotation.y, worldRotation.z);
            if (degrees >= 90)
                degrees = 90;
            else
                degrees += 1;
        }
        if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS)
        {
            // NEGATIVE ROTATION of object aling the X-AXIS
            worldRotation = vec3(1.0f, worldRotation.y, worldRotation.z);
            if (degrees <= -90)
                degrees = -90;
            else
                degrees -= 1;
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        {
            // POSITIVE ROTATION of object along the Y-AXIS
            worldRotation = vec3(worldRotation.x, 1.0f, worldRotation.z);
            if (degrees == 90)
                degrees = 90;
            else
                degrees += 1;
        }
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        {
            // NEGATIVE ROTATION of object aling the Y-AXIS
            worldRotation = vec3(worldRotation.x, 1.0f, worldRotation.z);
            if (degrees == -90)
                degrees = -90;
            else
                degrees -= 1;
        }
        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            // To Reset the racket orientation
            degrees = 0;
            worldRotation = vec3(0.0f, 1.0f, 0.0f);
        }

        /*******************
         * HOME | H BUTTON *
         *******************/
        // When the HOME button is pressed ----------------------------------------
        if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) // Reset camera to initial position
        {
            cameraPosition = vec3(1.0f, 3.0f, 10.0f); // EYE
            cameraLookAt = vec3(1.0f, 1.0f, -1.0f);   // AT
            cameraUp = vec3(0.0f, 1.0f, 0.0f);        // UP
        }
        // When the H button is pressed -----------------------------------------
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) // Reset camera to initial position
        {                                                 // Front Side View of Object Space
            // Same as when HOME Button is pressed
            cameraPosition = vec3(1.0f, 3.0f, 10.0f); // EYE
            cameraLookAt = vec3(1.0f, 1.0f, -1.0f);   // AT
            cameraUp = vec3(0.0f, 1.0f, 0.0f);        // UP
        }

        /*****************
         * MOUSE BUTTONS *
         *****************/
        // When Mouse buttons are pressed ---------------------------------------
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            // While left button is pressed -> use mouse movement to move into / out of the scene
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                cameraPosition += cameraPosition * dt * currentCameraSpeed; // Zoom in as mouse moves up
            else
                cameraPosition -= cameraPosition * dt * currentCameraSpeed; // Zoom out as mouse moves down
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            // While right button is pressed -> use mouse movement in the x direction to pan
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                cameraPosition -= cameraSideVector * dt * currentCameraSpeed; // pan camera to the left
            else
                cameraPosition += cameraSideVector * dt * currentCameraSpeed; // pan camera to the right
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
        {
            // While the middle button is pressed -> use mouse movement in the y direction to pan
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                cameraPosition.y += dt * currentCameraSpeed;
            else
                cameraPosition.y -= dt * currentCameraSpeed;
        }

        // Set the view matrix for first and third person cameras
        // - In first person, camera lookat is set like below
        // - In third person, camera position is on a sphere looking towards center
        viewMatrix = mat4(1.0f);

        if (cameraFirstPerson)
        {
            viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        }
        else
        {
            // Position of the camera is on the sphere looking at the point of interest (cameraPosition)
            float radius = 5.0f;
            vec3 position = cameraPosition - vec3(radius * cosf(phi) * cosf(theta),
                                                  radius * sinf(phi),
                                                  -radius * cosf(phi) * sinf(theta));
            viewMatrix = lookAt(position, cameraPosition, cameraUp);
        }

        setViewMatrix(colorShaderProgram, viewMatrix);
        setViewMatrix(texturedShaderProgram, viewMatrix);
    }
    // Close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}
GLuint setupModelVBO(string path, int &vertexCount)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> UVs;

    // read the vertex data from the model's OBJ file
    loadOBJ(path.c_str(), vertices, normals, UVs);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); // Becomes active VAO
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

    // Vertex VBO setup
    GLuint vertices_VBO;
    glGenBuffers(1, &vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // Normals VBO setup
    GLuint normals_VBO;
    glGenBuffers(1, &normals_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(1);

    // UVs VBO setup
    GLuint uvs_VBO;
    glGenBuffers(1, &uvs_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs, as we are using multiple VAOs)
    vertexCount = vertices.size();
    return VAO;
}

GLuint setupModelEBO(string path, int &vertexCount)
{
    vector<int> vertexIndices;
    // The contiguous sets of three indices of vertices, normals and UVs, used to make a triangle
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec2> UVs;

    // read the vertices from the cube.obj file
    // We won't be needing the normals or UVs for this program
    loadOBJ2(path.c_str(), vertexIndices, vertices, normals, UVs);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); // Becomes active VAO
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

    // Vertex VBO setup
    GLuint vertices_VBO;
    glGenBuffers(1, &vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // Normals VBO setup
    GLuint normals_VBO;
    glGenBuffers(1, &normals_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(1);

    // UVs VBO setup
    GLuint uvs_VBO;
    glGenBuffers(1, &uvs_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(2);

    // EBO setup
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(int), &vertexIndices.front(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    vertexCount = vertexIndices.size();
    return VAO;
}
