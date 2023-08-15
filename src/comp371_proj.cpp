/*
 * @file comp371_proj.cpp
 *
 * CONCORDIA UNIVERSITY
 * COMP 371: COMPUTER GRAPHICS
 * Section CC
 * Professor: Kaustubha Mendhurwar
 *
 * FINAL PROJECT
 * DUE DATE: MONDAY, AUGUST 14, 2023
 *
 * COMP 371 - Project (Summer 2023)
 * Name - Cristian Tesa
 * Student ID - 40157750
 */

#include <cstdlib> // needed for rand()
#include <iostream>
#include <vector>
#include <string>

#define GLEW_STATIC 1 // this allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>  // include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>                  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/gtc/type_ptr.hpp>         // this allows to use value_ptr which is used for specifying the appropriet color of each model part

#include "shaderloader.h" // this allows to read and use the shader .glsl files
#include "vertexData.h"   // this allows to easily reset object colors to cube model

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> // this allows to process the textures

#include "../include/modelDrawer.h"  // this allows drawing the object in the scene
#include "../include/modelObjects.h" // additional model objects

using namespace glm;
using namespace std;

// InitContext needed variables:
GLFWwindow *window = nullptr;
bool InitContext();

// loadTexture needed declaration:
GLuint loadTexture(const char *filename);

// Window dimensions:
const GLuint WIDTH = 1024, HEIGHT = 768;

// Skybox model:
float skyboxVertices[] = {

    // positions
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f};

int createTexturedCubeVertexArrayObject()
{

    // Create a vertex array:
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject):
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

    // Vertex setup:
    glVertexAttribPointer(0,                             // attribute 0 matches aPos in Vertex Shader
                          3,                             // size
                          GL_FLOAT,                      // type
                          GL_FALSE,                      // normalized?
                          sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color) and 1 vec2 (uv coordinates)
                          (void *)0                      // array buffer offset
    );

    glEnableVertexAttribArray(0);

    // Normals setup:
    glVertexAttribPointer(1, // attribute 1 matches aNormal in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void *)sizeof(vec3) // normal is offseted by a vec3 (comes after position)
    );

    glEnableVertexAttribArray(1);

    // The UV data is added to the VBO to specify where the UV coordinates are located:
    glVertexAttribPointer(2, // attribute 2 matches aUV in Vertex Shader
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void *)(2 * sizeof(vec3)) // uv is offseted by 2 vec3 (comes after position and normal)
    );

    glEnableVertexAttribArray(2);

    return vertexArrayObject;
}

int createSkyboxVertexArrayObject()
{

    // Create a vertex array:
    GLuint skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);

    // Upload Vertex Buffer to the GPU, keep a reference to it (skyboxVBO):
    GLuint skyboxVBO;
    glGenBuffers(1, &skyboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    // Vertex setup:
    glVertexAttribPointer(0,                 // attribute 0 matches aPos
                          3,                 // size
                          GL_FLOAT,          // type
                          GL_FALSE,          // normalized?
                          3 * sizeof(float), // stride - each vertex contain 2 vec3 (position, color) and 1 vec2 (uv coordinates)
                          (void *)0          // array buffer offset
    );

    glEnableVertexAttribArray(0);

    return skyboxVAO;
}

// Here the sphere is generated by specifying its radius and the number of sectors and stacks is should have.
// The idea is that any point on a sphere can be represented by the two angles, the sector (longitude) angle and the stack (latitude) angle.
// We then have to convert these angles to Cartesian coordinate system (x, y, z) to be able to draw them with OpenGL.
// Note that EBO is used to implement a sphere since drawing only by vertices is more complex and requires double drawing for common vertices!
// Most of the formulas where taken from - http://www.songho.ca/opengl/gl_sphere.html
GLuint setupSphereEBO(float radius, int sectorCount, int stackCount, int &vertexCount)
{

    // Vector of vectors designed to store the vertices, normals and UVs info of the sphere:
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<vec2> UVs;

    const double PI = 3.14159265358979323846;      // pi value
    float x, y, z;                                 // vertex position
    float nx, ny, nz, lengthInv = (1.0f / radius); // vertex normal
    float u, v;                                    // vertex texure coordinates

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;

    float sectorAngle, stackAngle; // sector (longitude) angle and stack (latitude) angle

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep; // starting from (pi / 2) to (-pi / 2) ---> from 90 degrees to -90 degrees

        // Can compute z right away since it only depends on stack angle:
        z = radius * sinf(stackAngle); // r * sin(stack angle)

        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep; // starting from 0 to 2pi ---> from 0 degrees to 360 degrees

            // Since now we know the sector angle, both x and y can be computed:
            x = (radius * cosf(stackAngle)) * cosf(sectorAngle); // r * cos(u) * cos(v)
            y = (radius * cosf(stackAngle)) * sinf(sectorAngle); // r * cos(u) * sin(v)

            // Vertex position (x, y, z):
            vertices.push_back(vec3(x, y, z));

            // Normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals.push_back(vec3(nx, ny, nz));

            // Vertex texture coordinates (u, v) range between [0, 1]:
            u = ((float)j / sectorCount);
            v = ((float)i / stackCount);
            UVs.push_back(vec2(u, v));
        }
    }

    // Now we have to generate a counter clock-wise index list of sphere triangles to be able to implement the sphere using EBO and these indices.
    // We can clearly see that to draw a sector from a particular stack two triangles will be needed.
    // Let the current index be e1 and the next index on the next stack be e2.
    // The counter clock-wise drawing order of the two needed triangles for this particular sector of the stack will be:
    //      1. e1 -> e2 -> (e1 + 1)
    //      2. (e1 + 1) -> e2 -> (e2 + 1)
    vector<int> indices;
    int e1, e2;
    for (int i = 0; i < stackCount; ++i)
    {
        e1 = i * (sectorCount + 1); // beginning of current stack
        e2 = e1 + sectorCount + 1;  // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++e1, ++e2)
        {

            // Two triangles per sector excluding first and last stacks (since these two are made up only by triangles).
            // 1. e1 -> e2 -> (e1 + 1):
            if (i != 0)
            {
                indices.push_back(e1);
                indices.push_back(e2);
                indices.push_back(e1 + 1);
            }

            // 2. (e1 + 1) -> e2 -> (e2 + 1):
            if (i != (stackCount - 1))
            {
                indices.push_back(e1 + 1);
                indices.push_back(e2);
                indices.push_back(e2 + 1);
            }
        }
    }

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex VBO setup:
    GLuint vertices_VBO;
    glGenBuffers(1, &vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // Normals VBO setup:
    GLuint normals_VBO;
    glGenBuffers(1, &normals_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(1);

    // UVs VBO setup:
    GLuint uvs_VBO;
    glGenBuffers(1, &uvs_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(vec2), &UVs.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(2);

    // EBO setup:
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices.front(), GL_STATIC_DRAW);

    // Unbind VAO (it's always a good thing to unbind any buffer / array to prevent strange bugs).
    // Remember: do NOT unbind the EBO, keep it bound to this VAO!
    glBindVertexArray(0);

    vertexCount = indices.size();

    return VAO;
}

// This function is designed to initialize the program:
bool InitContext()
{

    // Initialize GLFW and OpenGL version:
    glfwInit();

    // This can be ignored if used on Mac due to potential error associated with this specific implementation!
    // #if defined(PLATFORM_OSX)
    //     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    //     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // #else
    //   // On windows, we set OpenGL version to 2.1, to support more hardware
    //     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    //     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // #endif

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is WIDTH x HEIGHT (1024 x 768):
    window = glfwCreateWindow(WIDTH, HEIGHT, "COMP 371 - Project", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();

        return false;
    }

    return true;
}

// This function loads the textures:
GLuint loadTexture(const char *filename)
{

    // Step 1 - Create and bind textures:
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    assert(textureId != 0);

    glBindTexture(GL_TEXTURE_2D, textureId);

    // Step 2 - Set filter parameters:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Step 3 - Load textures with dimension data:
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (!data)
    {
        cerr << "Error::Texture could not load texture file:" << filename << endl;
        return 0;
    }

    // Step 4 - Upload the texture to the GPU:
    GLenum format = 0;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    // Step 5 - Free resources:
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureId;
}

// Shader variable setters:
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

// Loads a cubemap texture from 6 individual texture faces:
// Order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            cout << "Cubemap texture failed to load at path: " << faces[i] << endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

int main(int argc, char *argv[])
{
    if (!InitContext())
        return -1;

    // Load textures:
    GLuint clayTextureID = loadTexture("../assets/textures/clay.jpeg");
    GLuint courtTextureID = loadTexture("../assets/textures/court.jpg");
    GLuint glossyOrangeTextureID = loadTexture("../assets/textures/glossyOrange.jpeg");
    GLuint glossyBlueTextureID = loadTexture("../assets/textures/glossyBlue.jpeg");
    GLuint tennisBallGreenTextureID = loadTexture("../assets/textures/tennisBallGreen.jpeg");
    GLuint glossTextureID = loadTexture("../assets/textures/gloss.jpg");

    /**** CROWD | SCOREBOARD TEXTURES ****/
    GLuint crowdTextureID = loadTexture("../assets/textures/crowd.jpg");
    GLuint scoreboardTextureID = loadTexture("../assets/textures/scoreboard.jpg");

    // Load the skybox faces:
    std::vector<std::string> faces;

    faces.push_back("../assets/skybox/right.jpg");
    faces.push_back("../assets/skybox/left.jpg");
    faces.push_back("../assets/skybox/top.jpg");
    faces.push_back("../assets/skybox/bottom.jpg");
    faces.push_back("../assets/skybox/front.jpg");
    faces.push_back("../assets/skybox/back.jpg");

    unsigned int cubemapTexture = loadCubemap(faces);

    // Background color:
    glClearColor(0.13f, 0.22f, 0.19f, 1.0f);

    // Compile and link shaders here:
    string shaderPathPrefix = "../assets/shaders/";

    GLuint shaderProgram = loadSHADER(shaderPathPrefix + "vert.glsl", shaderPathPrefix + "frag.glsl");
    GLuint lightProgram = loadSHADER(shaderPathPrefix + "lightVert.glsl", shaderPathPrefix + "lightFrag.glsl");
    GLuint textureProgram = loadSHADER(shaderPathPrefix + "textureVert.glsl", shaderPathPrefix + "textureFrag.glsl");
    GLuint shadowProgram = loadSHADER(shaderPathPrefix + "shadowVert.glsl", shaderPathPrefix + "shadowFrag.glsl");
    GLuint skyboxProgram = loadSHADER(shaderPathPrefix + "skyboxVert.glsl", shaderPathPrefix + "skyboxFrag.glsl");

    // Define and upload geometry to the GPU here ...
    // Cube:
    int texturedCubeVAO = createTexturedCubeVertexArrayObject();

    // Sphere:
    int sphereVertexCount;
    int texturedSphereVAO = setupSphereEBO(1.0f, 30, 30, sphereVertexCount); // the generated sphere is based on the input radius, sector, and srack count

    // Skybox:
    int skyboxVAO = createSkyboxVertexArrayObject();

    // Dimensions of the shadow texture, which should cover the viewport window size and shouldn't be oversized and waste resources.
    const unsigned int DEPTH_MAP_TEXTURE_SIZE = 1024;

    // Variable storing index to texture used for shadow mapping:
    GLuint depth_map_texture;

    // Get the texture:
    glGenTextures(1, &depth_map_texture);

    // Bind the texture so the next glTex calls affect it.
    glBindTexture(GL_TEXTURE_2D, depth_map_texture);

    // Create the texture and specify its attributes, including width, height, components (only depth is stored, no color information).
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    // Set texture sampler parameters:
    // The two calls below tell the texture sampler inside the shader how to upsample and downsample the texture.
    // Here we choose the nearest filtering option, which means we just use the value of the closest pixel to the chosen image coordinate.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // The two calls below tell the texture sampler inside the shader how it should deal with texture coordinates outside of the [0, 1] range.
    // Here we decide to clamp the image to the edge.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Variable storing index to framebuffer used for shadow mapping.
    GLuint depth_map_fbo; // fbo - framebuffer object

    // Get the framebuffer:
    glGenFramebuffers(1, &depth_map_fbo);

    // Bind the framebuffer so the next glFramebuffer calls affect it.
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);

    // Attach the depth map texture to the depth map framebuffer:
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_texture, 0);

    glDrawBuffer(GL_NONE); // disable rendering colors, only write depth values

    // Model initial position coordinates:
    float x = -10.0f;
    float y = 0.0f;
    float z = 0.0f;

    // Model updated position coordinates after it is repositioned:
    int new_x = 0.0f;
    int new_z = 0.0f;

    // Scale factor by which the model is scaled:
    float s = 1.0f;

    // Rotation angle by which the model is rotated:
    float r = 0.0f;

    // Rotation angle by which the 1st joint is rotated over the z-axis:
    float r1stJoint = 0.0f;

    // Rotation angle by which the 2nd joint is rotated:
    float r2ndJointX = 0.0f; // over x-axis
    float r2ndJointY = 0.0f; // over y-axis
    float r2ndJointZ = 0.0f; // over z-axis

    // Rendering mode of the model:
    GLenum renderMode = GL_TRIANGLES;

    // Global Variables for 2nd Racket transformations
    vec3 racket2Translation = vec3(2.0f, 1.0f, 0.0f);
    float degrees = 0.0f;
    vec3 racket2Rotation = vec3(0.0f, 1.0f, 0.0f);
    vec3 racket2Scale = vec3(2.0f, 2.0f, 2.0f);

    // Keep track whether a key on the keyboard is pressed or not:
    bool isSpacePressed = false;
    bool isUPressed = false;
    bool isJPressed = false;
    bool isEPressed = false;
    bool isQPressed = false;
    bool textureEnable = false; // on if 'x' is pressed, off if 'z' is pressed
    bool shadowEnable = false;  // on if 'b' is pressed, off if 'v' is pressed
    bool is1Pressed = false;
    bool is2Pressed = false;
    bool is3Pressed = false;
    bool is4Pressed = false;
    bool is5Pressed = false;
    bool is6Pressed = false;
    bool is7Pressed = false;
    bool is8Pressed = false;

    // Camera parameters for view transform:
    vec3 cameraPosition(0.0f, 15.0f, 30.0f);
    vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Other camera parameters:
    float cameraSpeed = 1.0f;                  // the speed by which the camera moves
    float cameraFastSpeed = (2 * cameraSpeed); // the fast speed by which the camera moves when shift keys are pressed
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    float fov = 70.0f; // field of view

    // Set initial view matrix:
    // The virtual camera's space of focus is the world space origin.
    mat4 viewMatrix = lookAt(cameraPosition,                  // eye
                             (cameraPosition + cameraLookAt), // center
                             cameraUp);                       // up

    // Set view matrix:
    SetUniformMat4(shaderProgram, "view_matrix", viewMatrix);
    SetUniformMat4(lightProgram, "view_matrix", viewMatrix);
    SetUniformMat4(textureProgram, "view_matrix", viewMatrix);
    SetUniformMat4(shadowProgram, "view_matrix", viewMatrix);

    // Set view position:
    SetUniformVec3(shaderProgram, "view_position", cameraPosition);
    SetUniformVec3(textureProgram, "view_position", cameraPosition);

    // Set projection matrix for shader:
    // This program uses the perspective view to display all the objects.
    mat4 projectionMatrix = perspective(radians(fov),      // field of view in degrees
                                        (800.0f / 600.0f), // aspect ratio
                                        0.01f, 180.0f);    // near and far (near > 0)

    // Set projection matrix:
    SetUniformMat4(shaderProgram, "projection_matrix", projectionMatrix);
    SetUniformMat4(lightProgram, "projection_matrix", projectionMatrix);
    SetUniformMat4(textureProgram, "projection_matrix", projectionMatrix);
    SetUniformMat4(skyboxProgram, "projection_matrix", projectionMatrix);

    // Gets the location of each of the ambient, diffuse, specular and shininess coeffiecient for the proper shader:
    GLuint shading_ambient_strengthLocation = glGetUniformLocation(shaderProgram, "shading_ambient_strength");
    GLuint shading_diffuse_strengthhLocation = glGetUniformLocation(shaderProgram, "shading_diffuse_strength");
    GLuint shading_specular_strengthLocation = glGetUniformLocation(shaderProgram, "shading_specular_strength");
    GLuint shininessLocation = glGetUniformLocation(textureProgram, "shininess_material");

    // Get the location of the skybox texture for the skybox shader:
    GLuint skyboxLocation = glGetUniformLocation(skyboxProgram, "skybox");

    // Set the skybox texture value for the skybox shader:
    glUniform1i(skyboxLocation, 0);

    // Set light color:
    SetUniformVec3(shaderProgram, "light_color", vec3(1.0, 1.0, 1.0));
    SetUniformVec3(textureProgram, "light_color", vec3(1.0, 1.0, 1.0));

    // Enable hidden surface removal:
    // Enable backface culling:
    glEnable(GL_CULL_FACE);

    // Enable depth test:
    glEnable(GL_DEPTH_TEST);

    // For frame time:
    float lastFrameTime = glfwGetTime();
    double lastMousePosX, lastMousePosY;                      // variables needed to hold the info about the last mouse x and y positions
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY); // stores the last mouse x and y positions in the corresponding variables

    // Entering main loop:
    while (!glfwWindowShouldClose(window))
    {

        // Frame time calculation:
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Bind geometry:
        glBindVertexArray(texturedCubeVAO);

        // Light parameters:
        vec3 lightPosition = vec3(0.0f, 30.0f, 0.0f); // the location of the light in 3D space
        vec3 lightFocus(0.0, 0.0, -1.0);              // the point in 3D space the light "looks" at
        vec3 lightDirection = normalize(lightFocus - lightPosition);

        float lightNearPlane = 1.0f;
        float lightFarPlane = 180.0f;

        mat4 lightProjectionMatrix = frustum(-1.0f, 1.0f, -1.0f, 1.0f, lightNearPlane, lightFarPlane);

        mat4 lightViewMatrix = lookAt(lightPosition, lightFocus, vec3(0.0f, 1.0f, 0.0f));
        mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;

        // Set light space matrix on shaders that use it:
        SetUniformMat4(shaderProgram, "light_view_proj_matrix", lightSpaceMatrix);
        SetUniformMat4(textureProgram, "light_view_proj_matrix", lightSpaceMatrix);
        SetUniformMat4(shadowProgram, "light_view_proj_matrix", lightSpaceMatrix);

        // Set light position on shaders that use it:
        SetUniformVec3(shaderProgram, "light_position", lightPosition);
        SetUniformVec3(textureProgram, "light_position", lightPosition);

        // Declaration of matrices representing all the model parts:
        mat4 coordinateWorldMatrix = mat4(1.0f);   // matrix corresponding to the coordinate system axes
        mat4 lowerHandPartMatrix = mat4(1.0f);     // matrix corresponding to the lower hand part
        mat4 lowerHandGroupMatrix = mat4(1.0f);    // matrix corresponding to the lower hand group
        mat4 firstJointPartMatrix = mat4(1.0f);    // matrix corresponding to the 1st joint part
        mat4 firstJointGroupMatrix = mat4(1.0f);   // matrix corresponding to the 1st joint group
        mat4 upperHandPartMatrix = mat4(1.0f);     // matrix corresponding to the upper hand part
        mat4 upperHandGroupMatrix = mat4(1.0f);    // matrix corresponding to the upper hand group
        mat4 secondJointPartMatrix = mat4(1.0f);   // matrix corresponding to the 2nd joint part
        mat4 secondJointGroupMatrix = mat4(1.0f);  // matrix corresponding to the 2nd joint group
        mat4 tennisRacketPartMatrix = mat4(1.0f);  // matrix corresponding to the tennis racket part
        mat4 tennisRacketGroupMatrix = mat4(1.0f); // matrix corresponding to the tennis racket group
        mat4 ballPartMatrix = mat4(1.0f);          // matrix corresponding to the tennis ball part
        mat4 ballGroupMatrix = mat4(1.0f);         // matrix corresponding to the tennis group group
        mat4 worldMatrix = mat4(1.0f);

        // Shadow rendering:
        // Shadow rendering has two parts:
        // 1) Render shadow map:
        //    a) Use program for shadows
        //    b) Resize window coordinates to fix depth map output size
        //    c) Bind depth map framebuffer to output the depth values

        // Use proper shader:
        glUseProgram(shadowProgram);

        // Use proper image output size:
        glViewport(0, 0, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE);

        // Bind depth map texture as output framebuffer:
        glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);

        // Clear depth data on the framebuffer:
        glClear(GL_DEPTH_BUFFER_BIT);

        if (shadowEnable)
        {

            // Draw the light object:
            drawLight(shadowProgram, vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 30.0f, 0.0f));

            // Draw a world ground:
            drawGround(shadowProgram, vec3(125.0f, 0.01f, 60.0f), vec3(0.0f, -0.06f, 0.0f));

            // Draw court surface:
            drawCourt(shadowProgram, shadowProgram, vec3(76.0f, 0.01f, 34.0f), vec3(0.0f, -0.05f, 0.0f));

            // Draw tennis poles:
            drawPoles(shadowProgram, vec3(1.0f, 6.6f, 1.0f), vec3(0.0f, 3.3f, -21.5f));

            // Draw tennis pole net:
            drawPoleNet(shadowProgram, vec3(0.1f, 2.5f, 0.1f), vec3(12.5f, 3.3f, -21.5f));

            // Draw the coordinate system axes:
            coordinateWorldMatrix = mat4(1.0f); // matrix corresponding to the coordinate system axes

            // The x-axis model:
            coordinateWorldMatrix = translate(mat4(1.0f), vec3(2.5f, 0.025f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 0.05f, 0.05f));

            SetUniformMat4(shadowProgram, "world_matrix", coordinateWorldMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // The y-axis model:
            coordinateWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(0.05f, 5.0f, 0.05f));

            SetUniformMat4(shadowProgram, "world_matrix", coordinateWorldMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

            // The z-axis model:
            coordinateWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 0.025f, 2.5f)) * scale(mat4(1.0f), vec3(0.05f, 0.05f, 5.0f));

            SetUniformMat4(shadowProgram, "world_matrix", coordinateWorldMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Draw the lower arm:
            lowerHandPartMatrix = mat4(1.0f);  // matrix corresponding to the lower hand part
            lowerHandGroupMatrix = mat4(1.0f); // matrix corresponding to the lower hand group
            worldMatrix = mat4(1.0f);

            lowerHandPartMatrix = scale(mat4(1.0f), vec3(5.0f, 1.0f, 1.0f));

            lowerHandGroupMatrix *= translate(mat4(1.0f), vec3(x, y, z)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));

            // Model shadow manipulations:
            if (isSpacePressed)
                lowerHandGroupMatrix *= translate(mat4(1.0f), vec3(new_x, 0.0f, new_z));

            if (isUPressed)
                lowerHandGroupMatrix *= scale(mat4(1.0f), vec3(s, s, s));

            if (isJPressed)
            {
                if (s <= 0.0f) // make sure the scale s does not go into negative numbers resulting in the inversion of the model!
                    s = 0.0f;

                lowerHandGroupMatrix *= scale(mat4(1.0f), vec3(s, s, s));
            }

            if (isQPressed)
                lowerHandGroupMatrix *= rotate(mat4(1.0f), radians(r), vec3(0.0f, 1.0f, 0.0f));

            if (isEPressed)
                lowerHandGroupMatrix *= rotate(mat4(1.0f), radians(r), vec3(0.0f, 1.0f, 0.0f));

            worldMatrix = lowerHandGroupMatrix * lowerHandPartMatrix;

            SetUniformMat4(shadowProgram, "world_matrix", worldMatrix);

            glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

            // Draw the 1st joint:
            firstJointPartMatrix = mat4(1.0f);  // matrix corresponding to the 1st joint part
            firstJointGroupMatrix = mat4(1.0f); // matrix corresponding to the 1st joint group

            firstJointPartMatrix = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));

            firstJointGroupMatrix *= translate(mat4(1.0f), vec3(-2.5f, 0.0f, 0.0f));

            if (is1Pressed)
                firstJointGroupMatrix *= rotate(mat4(1.0f), radians(r1stJoint), vec3(0.0f, 0.0f, 1.0f));

            if (is2Pressed)
                firstJointGroupMatrix *= rotate(mat4(1.0f), radians(r1stJoint), vec3(0.0f, 0.0f, 1.0f));

            worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * firstJointPartMatrix;

            SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);

            glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

            // Draw the upper hand:
            upperHandPartMatrix = mat4(1.0f);  // matrix corresponding to the upper hand part
            upperHandGroupMatrix = mat4(1.0f); // matrix corresponding to the upper hand group

            upperHandPartMatrix = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));

            upperHandGroupMatrix *= translate(mat4(1.0f), vec3(-1.5f, 1.5f, 0.0f));

            worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * upperHandPartMatrix;

            SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);
            glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

            // Draw the 2nd joint:
            secondJointPartMatrix = mat4(1.0f);  // matrix corresponding to the 2nd joint part
            secondJointGroupMatrix = mat4(1.0f); // matrix corresponding to the 2nd joint group

            secondJointPartMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));

            secondJointGroupMatrix *= translate(mat4(1.0f), vec3(-2.0f, 2.0f, 0.0f)) * rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f));

            if (is3Pressed)
                secondJointGroupMatrix *= rotate(mat4(1.0f), radians(r2ndJointX), vec3(1.0f, 0.0f, 0.0f));

            if (is4Pressed)
                secondJointGroupMatrix *= rotate(mat4(1.0f), radians(r2ndJointX), vec3(1.0f, 0.0f, 0.0f));

            if (is5Pressed)
                secondJointGroupMatrix *= rotate(mat4(1.0f), radians(r2ndJointY), vec3(0.0f, 1.0f, 0.0f));

            if (is6Pressed)
                secondJointGroupMatrix *= rotate(mat4(1.0f), radians(r2ndJointY), vec3(0.0f, 1.0f, 0.0f));

            if (is7Pressed)
                secondJointGroupMatrix *= rotate(mat4(1.0f), radians(r2ndJointZ), vec3(0.0f, 0.0f, 1.0f));

            if (is8Pressed)
                secondJointGroupMatrix *= rotate(mat4(1.0f), radians(r2ndJointZ), vec3(0.0f, 0.0f, 1.0f));

            worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * secondJointPartMatrix;

            SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);

            glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

            // Draw the tennis racket:
            tennisRacketPartMatrix = mat4(1.0f);  // matrix corresponding to the tennis racket part
            tennisRacketGroupMatrix = mat4(1.0f); // matrix corresponding to the tennis racket group

            tennisRacketPartMatrix = scale(mat4(1.0f), vec3(0.9f, 9.0f, 0.9f));

            tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(-1.75f, 2.5f, 0.0f));

            worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

            SetUniformMat4(shadowProgram, "world_matrix", worldMatrix);

            glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

            tennisRacketPartMatrix = scale(mat4(1.0f), vec3(4.0f, 0.9f, 0.8f));

            tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(0.0f, 4.5f, 0.0f));

            worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

            SetUniformMat4(shadowProgram, "world_matrix", worldMatrix);

            glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

            tennisRacketPartMatrix = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.9f, 3.5f, 0.9f));

            tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(-3.0f, 1.0f, 0.0f));

            worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

            SetUniformMat4(shadowProgram, "world_matrix", worldMatrix);

            glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

            tennisRacketPartMatrix = rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.9f, 3.5f, 0.9f));

            tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(6.0f, 0.0f, 0.0f));

            worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

            SetUniformMat4(shadowProgram, "world_matrix", worldMatrix);

            glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

            tennisRacketPartMatrix = scale(mat4(1.0f), vec3(0.9f, 6.0f, 0.8f));

            tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(-7.0f, 4.0f, 0.0f));

            worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

            SetUniformMat4(shadowProgram, "world_matrix", worldMatrix);

            glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

            tennisRacketPartMatrix = scale(mat4(1.0f), vec3(0.9f, 6.0f, 0.8f));

            tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(8.0f, 0.0f, 0.0f));

            worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

            SetUniformMat4(shadowProgram, "world_matrix", worldMatrix);

            glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

            tennisRacketPartMatrix = rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.9f, 3.5f, 0.9f));

            tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(-7.0f, 4.0f, 0.0f));

            worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

            SetUniformMat4(shadowProgram, "world_matrix", worldMatrix);

            glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

            tennisRacketPartMatrix = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.9f, 3.5f, 0.9f));

            tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(6.0f, 0.0f, 0.0f));

            worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

            SetUniformMat4(shadowProgram, "world_matrix", worldMatrix);

            glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

            tennisRacketPartMatrix = scale(mat4(1.0f), vec3(4.0f, 0.9f, 0.8f));

            tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(-3.0f, 1.0f, 0.0f));

            worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

            SetUniformMat4(shadowProgram, "world_matrix", worldMatrix);

            glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

            // Draw the tennis racket net:
            for (float i = -3.0f; i <= 3.0f; i++) // draws the vertical lines of the tennis racket grid
            {
                tennisRacketPartMatrix = translate(mat4(1.0f), vec3(0.0f, -5.0f, 0.0f)) * translate(mat4(1.0f), vec3(i, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.05f, 9.20f, 0.05f));

                worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

                SetUniformMat4(shadowProgram, "world_matrix", worldMatrix);

                glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0
            }

            for (float i = 9.5f; i <= 17.5f; i++) // draws the horizontal lines of the tennis racket grid
            {
                tennisRacketPartMatrix = translate(mat4(1.0f), vec3(0.0f, -13.5f, 0.25f)) * translate(mat4(1.0f), vec3(0.0f, -5.0f, -0.25f)) * translate(mat4(1.0f), vec3(0.0f, i, 0.0f)) * scale(mat4(1.0f), vec3(7.20f, 0.05f, 0.05f));

                worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

                SetUniformMat4(shadowProgram, "world_matrix", worldMatrix);

                glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0
            }

            // Bind geometry:
            glBindVertexArray(texturedSphereVAO); // needed since we are no longer drawing cubes, we are now drawing spheres

            // Draw the tennis ball:
            ballPartMatrix = mat4(1.0f);  // matrix corresponding to the tennis ball part
            ballGroupMatrix = mat4(1.0f); // matrix corresponding to the tennis group group
            worldMatrix = mat4(1.0f);

            ballGroupMatrix *= translate(mat4(1.0f), vec3(0.0f, -4.0f, 4.0f));

            worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * ballGroupMatrix * ballPartMatrix;

            SetUniformMat4(shadowProgram, "world_matrix", worldMatrix);

            glDrawElements(renderMode, sphereVertexCount, GL_UNSIGNED_INT, 0);
        }

        // Unbind geometry:
        glBindVertexArray(0);

        // 2) Render scene:
        //      a) Bind the default framebuffer
        //      b) Just render like we normally do

        // Use proper image output size:
        // Side note: we get the size from the framebuffer instead of using WIDTH and HEIGHT because of a bug with highDPI displays.
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Bind screen as output framebuffer:
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Clear color and depth data on framebuffer:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind depth map texture, by default it is active
        // glActiveTexture(GL_TEXTURE0);

        // Bind geometry:
        glBindVertexArray(texturedCubeVAO);

        // Draw the light object:
        SetUniformVec3(lightProgram, "lightColor", vec3(1.0f, 1.0f, 1.0f)); // updates the color of the light object
        drawLight(lightProgram, vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 30.0f, 0.0f));

        // Draw a world ground:
        if (textureEnable)
        {

            // We also have to set the shading_ambient_strength, shading_diffuse_strength, shading_specular_strength,
            // and shininess uniforms for the textureProgram to correspond to clay properties:
            SetUniformVec3(textureProgram, "ambient_material", vec3(0.2f, 0.2f, 0.2f));
            SetUniformVec3(textureProgram, "diffuse_material", vec3(0.8f, 0.6f, 0.4f));
            SetUniformVec3(textureProgram, "specular_material", vec3(0.1f, 0.1f, 0.1f));
            glUniform1f(shininessLocation, 5.0f);

            glActiveTexture(GL_TEXTURE1);

            SetUniform1Value(textureProgram, "textureSampler", 1);
            glBindTexture(GL_TEXTURE_2D, clayTextureID);

            drawGround(textureProgram, vec3(125.0f, 0.01f, 60.0f), vec3(0.0f, -0.06f, 0.0f));
        }
        else
        {
            SetUniformVec3(shaderProgram, "object_color", vec3(0.4f, 0.46f, 0.23f)); // updates the color of the ground
            drawGround(shaderProgram, vec3(125.0f, 0.01f, 60.0f), vec3(0.0f, -0.06f, 0.0f));
        }

        // Draw court surface:
        if (textureEnable)
        {

            // We also have to set the shading_ambient_strength, shading_diffuse_strength, shading_specular_strength, and shininess uniforms for the textureProgram to correspond to grass properties:
            SetUniformVec3(textureProgram, "ambient_material", vec3(0.2f, 0.2f, 0.2f));
            SetUniformVec3(textureProgram, "diffuse_material", vec3(0.1f, 0.7f, 0.2f));
            SetUniformVec3(textureProgram, "specular_material", vec3(0.1f, 0.1f, 0.1f));
            glUniform1f(shininessLocation, 10.0f);

            glActiveTexture(GL_TEXTURE2);

            SetUniform1Value(textureProgram, "textureSampler", 2);
            glBindTexture(GL_TEXTURE_2D, courtTextureID);

            SetUniformVec3(shaderProgram, "object_color", vec3(1.0f, 1.0f, 1.0f)); // updates the color of the court lines

            drawCourt(textureProgram, shaderProgram, vec3(76.0f, 0.01f, 34.0f), vec3(0.0f, -0.05f, 0.0f));
        }
        else
        {
            SetUniformVec3(shaderProgram, "object_color", vec3(0.55f, 0.25f, 0.31f)); // updates the color of the court
            drawCourt(shaderProgram, shaderProgram, vec3(76.0f, 0.01f, 34.0f), vec3(0.0f, -0.05f, 0.0f));
        }

        // Draw tennis poles:
        SetUniformVec3(shaderProgram, "object_color", vec3(0.25f, 0.27f, 0.2f)); // updates the color of the tennis poles
        drawPoles(shaderProgram, vec3(1.0f, 6.6f, 1.0f), vec3(0.0f, 3.3f, -21.5f));

        // Draw tennis pole net:
        SetUniformVec3(shaderProgram, "object_color", vec3(0.7f, 0.74f, 0.98f)); // updates the color of the tennis net
        drawPoleNet(shaderProgram, vec3(0.1f, 2.5f, 0.1f), vec3(2.5f, 3.3f, -21.5f));

        // Draw the coordinate system axes:
        // Again this can be done using the cube model.
        // The cube model is scaled in the direction of the corresponding axis from the coordinate system.
        // Since the scaling happens in both negative and positive directions from the origin,
        // the cube should also be translated in the positive direction by half the amount it was scaled up.

        // The x-axis model:
        coordinateWorldMatrix = translate(mat4(1.0f), vec3(2.5f, 0.025f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 0.05f, 0.05f));

        SetUniformMat4(shaderProgram, "world_matrix", coordinateWorldMatrix);
        SetUniformVec3(shaderProgram, "object_color", vec3(1.0f, 0.0f, 0.0f)); // update the color of the x-axis
        glDrawArrays(GL_TRIANGLES, 0, 36);                                     // 36 vertices, starting at index 0

        // The y-axis model:
        coordinateWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(0.05f, 5.0f, 0.05f));

        SetUniformMat4(shaderProgram, "world_matrix", coordinateWorldMatrix);
        SetUniformVec3(shaderProgram, "object_color", vec3(0.0f, 1.0f, 0.0f)); // update the color of the y-axis
        glDrawArrays(GL_TRIANGLES, 0, 36);                                     // 36 vertices, starting at index 0

        // The z-axis model:
        coordinateWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 0.025f, 2.5f)) * scale(mat4(1.0f), vec3(0.05f, 0.05f, 5.0f));

        SetUniformMat4(shaderProgram, "world_matrix", coordinateWorldMatrix);
        SetUniformVec3(shaderProgram, "object_color", vec3(0.0f, 0.0f, 1.0f)); // update the color of the z-axis
        glDrawArrays(GL_TRIANGLES, 0, 36);                                     // 36 vertices, starting at index 0

        // The hand (lower and upper), the tennis racket, and the tennis ball:
        // The position of each piece will be computed using hierarchical modeling.
        // Therefore, the part and the group matrices of each component will be used.

        // Draw the lower arm:
        lowerHandGroupMatrix = mat4(1.0f); // matrix corresponding to the lower hand group

        // For now, only the lowerHandPartMatrix is applied, it specifies where the lower hand model lands in world space on it's own,
        // when no special transformation is applied by the groups or parents.
        lowerHandPartMatrix = scale(mat4(1.0f), vec3(5.0f, 1.0f, 1.0f));

        // Second, we take the result of that, and apply another transformation on top of it with the group matrix.
        // The group matrix is identical for all the parts.
        // It allows to treat all the parts as a single model that can be manipulated as one entity.
        // Here, rotation and translation are used to ensure that the whole model faces x-axis.
        // We also translate it by (y + 0.5f) in the y direction to ensure that the whole model is above the ground surface (does not go through it).
        lowerHandGroupMatrix *= translate(mat4(1.0f), vec3(x, (y + 0.5f), z)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));

        // Model manipulations:
        // If the space bar is pressed, we multiply the original group matrix by the translation of our model by the new x and z position.
        if (isSpacePressed)
            lowerHandGroupMatrix *= translate(mat4(1.0f), vec3(new_x, 0.0f, new_z));

        // If key 'u' is pressed, we multiply the original group matrix by scaling up our model by the s factor.
        if (isUPressed)
            lowerHandGroupMatrix *= scale(mat4(1.0f), vec3(s, s, s));

        // If key 'j' is pressed, we multiply the original group matrix by scaling down our model by the s factor.
        if (isJPressed)
        {
            if (s <= 0.0f) // make sure the scale s does not go into negative numbers resulting in the inversion of the model!
                s = 0.0f;

            lowerHandGroupMatrix *= scale(mat4(1.0f), vec3(s, s, s));
        }

        // If key 'q' is pressed,
        // we multiply the original group matrix by the rotation of our model by r degrees in the left direction of the y-axis.
        if (isQPressed)
            lowerHandGroupMatrix *= rotate(mat4(1.0f), radians(r), vec3(0.0f, 1.0f, 0.0f));

        // If key 'e' is pressed,
        // we multiply the original group matrix by the rotation of our model by r degrees in the right direction of the y-axis.
        if (isEPressed)
            lowerHandGroupMatrix *= rotate(mat4(1.0f), radians(r), vec3(0.0f, 1.0f, 0.0f));

        worldMatrix = lowerHandGroupMatrix * lowerHandPartMatrix;

        SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);
        SetUniformVec3(shaderProgram, "object_color", vec3(0.94, 0.82, 0.59)); // update the color of the lower hand

        // Using renderMode, which allows to switch between different rendering modes when pressing a specific key on the keyboard.
        glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

        // Draw the 1st joint:
        firstJointGroupMatrix = mat4(1.0f);

        // For now, only the firstJointPartMatrix is applied, it specifies where the 1st joint model lands in world space on it's own,
        // when no special transformation is applied by the groups or parents.
        firstJointPartMatrix = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));

        // Second, we take the result of that, and apply another transformation on top of it with the group matrix.
        // The group matrix is identical for all the parts.
        // It allows to treat all the parts as a single model that can be manipulated as one entity.
        // Here, translation is used to ensure that the whole model faces x-axis.
        firstJointGroupMatrix *= translate(mat4(1.0f), vec3(-2.5f, 0.0f, 0.0f));

        // If key '1' is pressed,
        // we multiply the original group matrix by the rotation of our model by -r1stJoint degrees in the direction of the z-axis.
        if (is1Pressed)
            firstJointGroupMatrix *= rotate(mat4(1.0f), radians(r1stJoint), vec3(0.0f, 0.0f, 1.0f));

        // If key '2' is pressed,
        // we multiply the original group matrix by the rotation of our model by r1stJoint degrees in the right direction of the z-axis.
        if (is2Pressed)
            firstJointGroupMatrix *= rotate(mat4(1.0f), radians(r1stJoint), vec3(0.0f, 0.0f, 1.0f));

        worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * firstJointPartMatrix;

        SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);

        // Using renderMode, which allows to switch between different rendering modes when pressing a specific key on the keyboard.
        glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

        // Draw the upper hand:
        upperHandGroupMatrix = mat4(1.0f); // matrix corresponding to the upper hand group

        upperHandPartMatrix = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));

        // Note that the position of the upper hand depends on the position of the lower hand,
        // therefore, we have to translate the upper hand w.r.t. the lower hand that they are placed correctly in the world space (group matrix is used for this).
        upperHandGroupMatrix *= translate(mat4(1.0f), vec3(-1.5f, 1.5f, 0.0f));

        // The world matrix is computed using the upper hand part matrix, upper hand group matrix matrix,
        // and the parent(s) group matrix (1st joint group and lower hand group matrices).
        worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * upperHandPartMatrix;

        SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);

        glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

        // Draw the 2nd joint:
        secondJointGroupMatrix = mat4(1.0f);

        secondJointPartMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));

        secondJointGroupMatrix *= translate(mat4(1.0f), vec3(-2.0f, 2.0f, 0.0f)) * rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f));

        // If key '3' is pressed,
        // we multiply the original group matrix by the rotation of our model by -r2ndJointX degrees in the direction of the x-axis.
        if (is3Pressed)
            secondJointGroupMatrix *= rotate(mat4(1.0f), radians(r2ndJointX), vec3(1.0f, 0.0f, 0.0f));

        // If key '4' is pressed,
        // we multiply the original group matrix by the rotation of our model by r2ndJointX degrees in the direction of the x-axis.
        if (is4Pressed)
            secondJointGroupMatrix *= rotate(mat4(1.0f), radians(r2ndJointX), vec3(1.0f, 0.0f, 0.0f));

        // If key '5' is pressed,
        // we multiply the original group matrix by the rotation of our model by -r2ndJointY degrees in the direction of the y-axis.
        if (is5Pressed)
            secondJointGroupMatrix *= rotate(mat4(1.0f), radians(r2ndJointY), vec3(0.0f, 1.0f, 0.0f));

        // If key '6' is pressed,
        // we multiply the original group matrix by the rotation of our model by r2ndJointY degrees in the direction of the y-axis.
        if (is6Pressed)
            secondJointGroupMatrix *= rotate(mat4(1.0f), radians(r2ndJointY), vec3(0.0f, 1.0f, 0.0f));

        // If key '7' is pressed,
        // we multiply the original group matrix by the rotation of our model by -r2ndJointZ degrees in the direction of the z-axis.
        if (is7Pressed)
            secondJointGroupMatrix *= rotate(mat4(1.0f), radians(r2ndJointZ), vec3(0.0f, 0.0f, 1.0f));

        // If key '8' is pressed,
        // we multiply the original group matrix by the rotation of our model by r2ndJointZ degrees in the direction of the z-axis.
        if (is8Pressed)
            secondJointGroupMatrix *= rotate(mat4(1.0f), radians(r2ndJointZ), vec3(0.0f, 0.0f, 1.0f));

        worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * secondJointPartMatrix;

        SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);

        glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

        // Draw the tennis racket:
        tennisRacketGroupMatrix = mat4(1.0f);

        // The tennisRacketPartMatrix specifies where the tennis racket model lands in world space on it's own,
        // when no special transformation is applied by the groups or parents (the lower arm and the upper arm).
        tennisRacketPartMatrix = scale(mat4(1.0f), vec3(0.9f, 9.0f, 0.9f));

        // Here, translation is applied to ensure that the tennis racket is correctly placed w.r.t. the upper, lower hands, and their corresponding joints.
        tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(-0.5f, 0.5f, 0.0f));

        // The world matrix is computed using the tennis racket part matrix, tennis racket group matrix,
        // and the parents group matrices (2nd joint, upper, 1st joint, and lower hand group matrices).
        worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

        if (textureEnable)
        {

            // Draw textured geometry:
            glUseProgram(textureProgram);

            // We also have to set the shading_ambient_strength, shading_diffuse_strength, shading_specular_strength, and shininess uniforms for the textureProgram to correspond to glossy finish properties:
            SetUniformVec3(textureProgram, "ambient_material", vec3(0.2f, 0.2f, 0.2f));
            SetUniformVec3(textureProgram, "diffuse_material", vec3(1.0f, 0.8f, 0.9f));
            SetUniformVec3(textureProgram, "specular_material", vec3(0.9f, 0.9f, 0.9f));
            glUniform1f(shininessLocation, 50.0f);

            glActiveTexture(GL_TEXTURE1);
            SetUniform1Value(textureProgram, "textureSampler", 1);
            glBindTexture(GL_TEXTURE_2D, glossTextureID);
            // Draw the 2nd racket
            draw_racket2(textureProgram, racket2Translation, degrees - 90.0, racket2Rotation, racket2Scale, renderMode);

            SetUniform1Value(textureProgram, "textureSampler", 1);
            glBindTexture(GL_TEXTURE_2D, glossyOrangeTextureID);

            SetUniformMat4(textureProgram, "world_matrix", worldMatrix);
        }
        else
        {

            // Draw proper geometry:
            glUseProgram(shaderProgram);

            SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);
            SetUniformVec3(shaderProgram, "object_color", vec3(0.93, 0.32, 0.099)); // update the color of the tennis racket
        }

        glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0.

        // The tennis racket consists of multiple parts, so each of them must be drawn.
        tennisRacketPartMatrix = scale(mat4(1.0f), vec3(4.0f, 0.9f, 0.8f));

        tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(0.0f, 4.5f, 0.0f));

        worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

        if (textureEnable)
        {

            // Draw textured geometry:
            glUseProgram(textureProgram);

            // We also have to set the shading_ambient_strength, shading_diffuse_strength, shading_specular_strength, and shininess uniforms for the textureProgram to correspond to glossy finish properties:
            SetUniformVec3(textureProgram, "ambient_material", vec3(0.2f, 0.2f, 0.2f));
            SetUniformVec3(textureProgram, "diffuse_material", vec3(0.2f, 0.4f, 1.0f));
            SetUniformVec3(textureProgram, "specular_material", vec3(0.8f, 0.8f, 0.8f));
            glUniform1f(shininessLocation, 80.0f);

            glActiveTexture(GL_TEXTURE2);

            SetUniform1Value(textureProgram, "textureSampler", 2);
            glBindTexture(GL_TEXTURE_2D, glossyBlueTextureID);

            SetUniformMat4(textureProgram, "world_matrix", worldMatrix);
        }
        else
        {

            // Draw proper geometry:
            glUseProgram(shaderProgram);

            SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);
            SetUniformVec3(shaderProgram, "object_color", vec3(0.28, 0.71, 0.094)); // update the color of the tennis racket
        }

        glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

        tennisRacketPartMatrix = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.9f, 3.5f, 0.9f));

        tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(-3.0f, 1.0f, 0.0f));

        worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

        if (textureEnable)
        {

            // Draw textured geometry:
            glUseProgram(textureProgram);

            // We also have to set the shading_ambient_strength, shading_diffuse_strength, shading_specular_strength, and shininess uniforms for the textureProgram to correspond to glossy finish properties:
            SetUniformVec3(textureProgram, "ambient_material", vec3(0.2f, 0.2f, 0.2f));
            SetUniformVec3(textureProgram, "diffuse_material", vec3(1.0f, 0.8f, 0.9f));
            SetUniformVec3(textureProgram, "specular_material", vec3(0.9f, 0.9f, 0.9f));
            glUniform1f(shininessLocation, 50.0f);

            glActiveTexture(GL_TEXTURE1);

            SetUniform1Value(textureProgram, "textureSampler", 1);
            glBindTexture(GL_TEXTURE_2D, glossyOrangeTextureID);

            SetUniformMat4(textureProgram, "world_matrix", worldMatrix);
        }
        else
        {

            // Draw proper geometry:
            glUseProgram(shaderProgram);

            SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);
            SetUniformVec3(shaderProgram, "object_color", vec3(0.93, 0.32, 0.099)); // update the color of the tennis racket
        }

        glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

        tennisRacketPartMatrix = rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.9f, 3.5f, 0.9f));

        tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(6.0f, 0.0f, 0.0f));

        worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

        if (textureEnable)
        {

            // Draw textured geometry:
            glUseProgram(textureProgram);

            // We also have to set the shading_ambient_strength, shading_diffuse_strength, shading_specular_strength, and shininess uniforms for the textureProgram to correspond to glossy finish properties:
            SetUniformVec3(textureProgram, "ambient_material", vec3(0.2f, 0.2f, 0.2f));
            SetUniformVec3(textureProgram, "diffuse_material", vec3(1.0f, 0.8f, 0.9f));
            SetUniformVec3(textureProgram, "specular_material", vec3(0.9f, 0.9f, 0.9f));
            glUniform1f(shininessLocation, 50.0f);

            glActiveTexture(GL_TEXTURE1);

            SetUniform1Value(textureProgram, "textureSampler", 1);
            glBindTexture(GL_TEXTURE_2D, glossyOrangeTextureID);

            SetUniformMat4(textureProgram, "world_matrix", worldMatrix);
        }
        else
        {

            // Draw proper geometry:
            glUseProgram(shaderProgram);

            SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);
            SetUniformVec3(shaderProgram, "object_color", vec3(0.93, 0.32, 0.099)); // update the color of the tennis racket
        }

        glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

        tennisRacketPartMatrix = scale(mat4(1.0f), vec3(0.9f, 6.0f, 0.8f));

        tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(-7.0f, 4.0f, 0.0f));

        worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

        if (textureEnable)
        {

            // Draw textured geometry:
            glUseProgram(textureProgram);

            // We also have to set the shading_ambient_strength, shading_diffuse_strength, shading_specular_strength, and shininess uniforms for the textureProgram to correspond to glossy finish properties:
            SetUniformVec3(textureProgram, "ambient_material", vec3(0.2f, 0.2f, 0.2f));
            SetUniformVec3(textureProgram, "diffuse_material", vec3(0.2f, 0.4f, 1.0f));
            SetUniformVec3(textureProgram, "specular_material", vec3(0.8f, 0.8f, 0.8f));
            glUniform1f(shininessLocation, 80.0f);

            glActiveTexture(GL_TEXTURE2);

            SetUniform1Value(textureProgram, "textureSampler", 2);
            glBindTexture(GL_TEXTURE_2D, glossyBlueTextureID);

            SetUniformMat4(textureProgram, "world_matrix", worldMatrix);
        }
        else
        {

            // Draw proper geometry:
            glUseProgram(shaderProgram);

            SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);
            SetUniformVec3(shaderProgram, "object_color", vec3(0.28, 0.71, 0.094)); // update the color of the tennis racket
        }

        glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

        tennisRacketPartMatrix = scale(mat4(1.0f), vec3(0.9f, 6.0f, 0.8f));

        tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(8.0f, 0.0f, 0.0f));

        worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

        if (textureEnable)
        {

            // Draw textured geometry:
            glUseProgram(textureProgram);

            // We also have to set the shading_ambient_strength, shading_diffuse_strength, shading_specular_strength, and shininess uniforms for the textureProgram to correspond to glossy finish properties:
            SetUniformVec3(textureProgram, "ambient_material", vec3(0.2f, 0.2f, 0.2f));
            SetUniformVec3(textureProgram, "diffuse_material", vec3(0.2f, 0.4f, 1.0f));
            SetUniformVec3(textureProgram, "specular_material", vec3(0.8f, 0.8f, 0.8f));
            glUniform1f(shininessLocation, 80.0f);

            glActiveTexture(GL_TEXTURE2);

            SetUniform1Value(textureProgram, "textureSampler", 2);
            glBindTexture(GL_TEXTURE_2D, glossyBlueTextureID);

            SetUniformMat4(textureProgram, "world_matrix", worldMatrix);
        }
        else
        {

            // Draw proper geometry:
            glUseProgram(shaderProgram);

            SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);
            SetUniformVec3(shaderProgram, "object_color", vec3(0.28, 0.71, 0.094)); // update the color of the tennis racket
        }

        glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

        tennisRacketPartMatrix = rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.9f, 3.5f, 0.9f));

        tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(-7.0f, 4.0f, 0.0f));

        worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

        if (textureEnable)
        {

            // Draw textured geometry:
            glUseProgram(textureProgram);

            // We also have to set the shading_ambient_strength, shading_diffuse_strength, shading_specular_strength, and shininess uniforms for the textureProgram to correspond to glossy finish properties:
            SetUniformVec3(textureProgram, "ambient_material", vec3(0.2f, 0.2f, 0.2f));
            SetUniformVec3(textureProgram, "diffuse_material", vec3(1.0f, 0.8f, 0.9f));
            SetUniformVec3(textureProgram, "specular_material", vec3(0.9f, 0.9f, 0.9f));
            glUniform1f(shininessLocation, 50.0f);

            glActiveTexture(GL_TEXTURE1);

            SetUniform1Value(textureProgram, "textureSampler", 1);
            glBindTexture(GL_TEXTURE_2D, glossyOrangeTextureID);

            SetUniformMat4(textureProgram, "world_matrix", worldMatrix);
        }
        else
        {

            // Draw proper geometry:
            glUseProgram(shaderProgram);

            SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);
            SetUniformVec3(shaderProgram, "object_color", vec3(0.93, 0.32, 0.099)); // update the color of the tennis racket
        }

        glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

        tennisRacketPartMatrix = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.9f, 3.5f, 0.9f));

        tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(6.0f, 0.0f, 0.0f));

        worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

        if (textureEnable)
        {

            // Draw textured geometry:
            glUseProgram(textureProgram);

            // We also have to set the shading_ambient_strength, shading_diffuse_strength, shading_specular_strength, and shininess uniforms for the textureProgram to correspond to glossy finish properties:
            SetUniformVec3(textureProgram, "ambient_material", vec3(0.2f, 0.2f, 0.2f));
            SetUniformVec3(textureProgram, "diffuse_material", vec3(1.0f, 0.8f, 0.9f));
            SetUniformVec3(textureProgram, "specular_material", vec3(0.9f, 0.9f, 0.9f));
            glUniform1f(shininessLocation, 50.0f);

            glActiveTexture(GL_TEXTURE1);

            SetUniform1Value(textureProgram, "textureSampler", 1);
            glBindTexture(GL_TEXTURE_2D, glossyOrangeTextureID);

            SetUniformMat4(textureProgram, "world_matrix", worldMatrix);
        }
        else
        {

            // Draw proper geometry:
            glUseProgram(shaderProgram);

            SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);
            SetUniformVec3(shaderProgram, "object_color", vec3(0.93, 0.32, 0.099)); // update the color of the tennis racket
        }

        glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

        tennisRacketPartMatrix = scale(mat4(1.0f), vec3(4.0f, 0.9f, 0.8f));

        tennisRacketGroupMatrix *= translate(mat4(1.0f), vec3(-3.0f, 1.0f, 0.0f));

        worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

        if (textureEnable)
        {

            // Draw textured geometry:
            glUseProgram(textureProgram);

            // We also have to set the shading_ambient_strength, shading_diffuse_strength, shading_specular_strength, and shininess uniforms for the textureProgram to correspond to glossy finish properties:
            SetUniformVec3(textureProgram, "ambient_material", vec3(0.2f, 0.2f, 0.2f));
            SetUniformVec3(textureProgram, "diffuse_material", vec3(0.2f, 0.4f, 1.0f));
            SetUniformVec3(textureProgram, "specular_material", vec3(0.8f, 0.8f, 0.8f));
            glUniform1f(shininessLocation, 80.0f);

            glActiveTexture(GL_TEXTURE2);

            SetUniform1Value(textureProgram, "textureSampler", 2);
            glBindTexture(GL_TEXTURE_2D, glossyBlueTextureID);

            SetUniformMat4(textureProgram, "world_matrix", worldMatrix);
        }
        else
        {

            // Draw proper geometry:
            glUseProgram(shaderProgram);

            SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);
            SetUniformVec3(shaderProgram, "object_color", vec3(0.28, 0.71, 0.094)); // update the color of the tennis racket
        }

        glDrawArrays(renderMode, 0, 36); // 36 vertices, starting at index 0

        // Draw proper geometry:
        glUseProgram(shaderProgram);

        // Draw the tennis racket net:
        for (float i = -3.0f; i <= 3.0f; i++) // draws the vertical lines of the tennis racket grid
        {
            tennisRacketPartMatrix = translate(mat4(1.0f), vec3(0.0f, -5.0f, 0.0f)) * translate(mat4(1.0f), vec3(i, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.05f, 9.20f, 0.05f));

            worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

            SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);
            SetUniformVec3(shaderProgram, "object_color", vec3(1.0, 1.0, 1.0)); // update the color of the tennis net
            glDrawArrays(renderMode, 0, 36);                                    // 36 vertices, starting at index 0
        }

        for (float i = 9.5f; i <= 17.5f; i++) // draws the horizontal lines of the tennis racket grid
        {
            tennisRacketPartMatrix = translate(mat4(1.0f), vec3(0.0f, -13.5f, 0.25f)) * translate(mat4(1.0f), vec3(0.0f, -5.0f, -0.25f)) * translate(mat4(1.0f), vec3(0.0f, i, 0.0f)) * scale(mat4(1.0f), vec3(7.20f, 0.05f, 0.05f));

            worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * tennisRacketPartMatrix;

            SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);
            SetUniformVec3(shaderProgram, "object_color", vec3(1.0, 1.0, 1.0)); // update the color of the tennis net
            glDrawArrays(renderMode, 0, 36);                                    // 36 vertices, starting at index 0
        }

        // Draw the 2nd racket
        draw_racket2(shaderProgram, racket2Translation, degrees - 90.0, racket2Rotation, racket2Scale, renderMode);

        // Bind geometry:
        glBindVertexArray(texturedSphereVAO); // needed since we are no longer drawing cubes, we are now drawing spheres

        // Draw the tennis ball
        ballGroupMatrix = mat4(1.0f);

        ballGroupMatrix *= translate(mat4(1.0f), vec3(0.0f, -4.0f, 4.0f));

        worldMatrix = lowerHandGroupMatrix * firstJointGroupMatrix * upperHandGroupMatrix * secondJointGroupMatrix * tennisRacketGroupMatrix * ballGroupMatrix * ballPartMatrix;

        if (textureEnable)
        {

            // Draw textured geometry:
            glUseProgram(textureProgram);

            // We also have to set the shading_ambient_strength, shading_diffuse_strength, shading_specular_strength, and shininess uniforms for the textureProgram to correspond to tennis ball texture properties:
            SetUniformVec3(textureProgram, "ambient_material", vec3(0.2f, 0.2f, 0.2f));
            SetUniformVec3(textureProgram, "diffuse_material", vec3(0.1f, 0.7f, 0.2f));
            SetUniformVec3(textureProgram, "specular_material", vec3(0.7f, 0.7f, 0.7f));
            glUniform1f(shininessLocation, 5.0f);

            glActiveTexture(GL_TEXTURE3);

            SetUniform1Value(textureProgram, "textureSampler", 3);
            glBindTexture(GL_TEXTURE_2D, tennisBallGreenTextureID);

            SetUniformMat4(textureProgram, "world_matrix", worldMatrix);
        }
        else
        {

            // Draw proper geometry:
            glUseProgram(shaderProgram);

            SetUniformVec3(shaderProgram, "object_color", vec3(0.75, 0.58, 0.35)); // update the color of the tennis ball
            SetUniformMat4(shaderProgram, "world_matrix", worldMatrix);
        }

        glDrawElements(renderMode, sphereVertexCount, GL_UNSIGNED_INT, 0);

        // Unbind geometry:
        glBindVertexArray(0);

        // Draw skybox as last:
        glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
        glUseProgram(skyboxProgram);
        viewMatrix = mat4(mat3(lookAt(cameraPosition, (cameraPosition + cameraLookAt), cameraUp))); // remove translation from the view matrix
        SetUniformMat4(skyboxProgram, "view_matrix", viewMatrix);

        // Skybox cube:
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // Disable mouse cursor:
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // End frame:
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Handle inputs:
        // Exit the program:
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Re-position the model at a random location on the grid:
        if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS))
        {
            isSpacePressed = true; // activates the code availabe only when the space bar is pressed

            // Randomly computes the new x and z coordinates of the model:
            new_x = ((rand() % (40 - (-40) + 1)) + (-40));
            new_z = ((rand() % (40 - (-40) + 1)) + (-40));
        }

        // Scale up the model:
        if ((glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS))
        {
            isUPressed = true; // activates the code availabe only when the 'u' key is pressed
            s += 0.1;          // increase the scale factor s by 0.1
            racket2Scale /= 0.99;
        }

        // Scale down the model:
        if ((glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS))
        {
            isJPressed = true; // activates the code availabe only when the 'j' key is pressed
            s -= 0.1;          // decrease the scale factor s by 0.1
            racket2Scale *= 0.99;
        }

        // Move the model to the left:
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            x -= 0.1; // decrease the x coordinate position by 0.1

        // Move the model to the right:
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            x += 0.1; // increase the x coordinate position by 0.1

        // Move the model up:
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            y += 0.1; // increase the y coordinate position by 0.1

        // Move the model down:
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            y -= 0.1; // decrease the y coordinate position by 0.1

        // Rotate the model left 5 degrees about the y-axis:
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            isQPressed = true; // activates the code availabe only when the 'q' key is pressed
            r -= 5.0f;         // decrease the rotation angle r by 5.0f degrees
        }

        // Rotate the model right 5 degrees about the y-axis:
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            isEPressed = true; // activates the code availabe only when the 'e' key is pressed
            r += 5.0f;         // increase the rotation angle r by 5.0f degrees
        }

        // Render the model using points:
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
            renderMode = GL_POINTS; // changes the rendering mode to render points

        // Render the model using lines:
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
            renderMode = GL_LINES; // changes the rendering mode to render lines

        // Render the model using triangles:
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
            renderMode = GL_TRIANGLES; // changes the rendering mode to render triangles

        // Enables the textures:
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
            textureEnable = true;

        // Disables the textures:
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
            textureEnable = false;

        // Enables the shadows:
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
            shadowEnable = true;

        // Disables the shadows:
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
            shadowEnable = false;

        // Rotate the 1st joint -1 degree about the z-axis:
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            is1Pressed = true; // activates the code availabe only when the '1' key is pressed
            r1stJoint -= 1.0f; // decrease the rotation angle r1stJoint by 1.0f degree
        }

        // Rotate the 1st joint +1 degree about the z-axis:
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            is2Pressed = true; // activates the code availabe only when the '2' key is pressed
            r1stJoint += 1.0f; // increase the rotation angle r1stJoint by 1.0f degree
        }

        // Rotate the 2nd joint -1 degree about the x-axis:
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            is3Pressed = true;  // activates the code availabe only when the '3' key is pressed
            r2ndJointX -= 1.0f; // decrease the rotation angle r2ndJointX by 1.0f degree
        }

        // Rotate the 2nd joint -1 degree about the x-axis:
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            is4Pressed = true;  // activates the code availabe only when the '2' key is pressed
            r2ndJointX += 1.0f; // increase the rotation angle r2ndJointX by 1.0f degree
        }

        // Rotate the 2nd joint -1 degree about the y-axis:
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            is5Pressed = true;  // activates the code availabe only when the '5' key is pressed
            r2ndJointY -= 1.0f; // decrease the rotation angle r2ndJointY by 1.0f degree
        }

        // Rotate the 2nd joint -1 degree about the y-axis:
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {
            is6Pressed = true;  // activates the code availabe only when the '6' key is pressed
            r2ndJointY += 1.0f; // increase the rotation angle r2ndJointY by 1.0f degree
        }

        // Rotate the 2nd joint -1 degree about the z-axis:
        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
        {
            is7Pressed = true;  // activates the code availabe only when the '7' key is pressed
            r2ndJointZ -= 1.0f; // decrease the rotation angle r2ndJointZ by 1.0f degree
        }

        // Rotate the 2nd joint -1 degree about the y-axis:
        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
        {
            is8Pressed = true;  // activates the code availabe only when the '8' key is pressed
            r2ndJointZ += 1.0f; // increase the rotation angle r2ndJointZ by 1.0f degree
        }
        /************** KEYBOARD Input Rotations for 2nd Racket ******************
         * COMMA | PERIOD | SEMICOLON | SLASH | LEFT_BRACKET | APOSTROPHE |
         * RIGHT_BRACKET | BACKSLASH | MINUS | EQUAL BUTTONS
         ***********************************************************************/

        // RACKET TRANSLATIONS
        if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
            racket2Translation.x += 0.1; // POSTIVE TRANSLATION of object along the X-AXIS
        if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
            racket2Translation.x -= 0.1; // NEGATIVE TRANSLATION of object along the X-AXIS
        if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS)
            racket2Translation.y += 0.1; // POSITIVE TRANSLATION of object along the Y-AXIS
        if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS)
            racket2Translation.y -= 0.1; // NEGATIVE TRANSLATION of object aling the Y-AXIS

        // RACKET ROTATIONS
        if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
        { // POSITIVE ROTATION of object along the Y-AXIS
            racket2Rotation = vec3(racket2Rotation.x * 0, 1.0f, racket2Rotation.z * 0);
            degrees += 5;
        }
        if (glfwGetKey(window, GLFW_KEY_APOSTROPHE) == GLFW_PRESS)
        { // NEGATIVE ROTATION of object aling the Y-AXIS
            racket2Rotation = vec3(racket2Rotation.x * 0, 1.0f, racket2Rotation.z * 0);
            degrees -= 5;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
        {
            // POSTIVE ROTATATION of object along the Z-AXIS
            racket2Rotation = vec3(racket2Rotation.x * 0, racket2Rotation.y * 0, 1.0f);
            degrees += 1;
        }
        if (glfwGetKey(window, GLFW_KEY_BACKSLASH) == GLFW_PRESS)
        {
            // NEGATIVE ROTATION of object along the Z-AXIS
            racket2Rotation = vec3(racket2Rotation.x * 0, racket2Rotation.y * 0, 1.0f);
            degrees -= 1;
        }
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
        {
            // POSITIVE ROTATION of object along the X-AXIS
            racket2Rotation = vec3(1.0f, racket2Rotation.y * 0, racket2Rotation.z * 0);
            degrees += 1;
        }
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
        {
            // NEGATIVE ROTATION of object aling the X-AXIS
            racket2Rotation = vec3(1.0f, racket2Rotation.y * 0, racket2Rotation.z * 0);
            degrees -= 1;
        }
        /**************************************************************************************/

        // Moving camera:
        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;

        // Calculate mouse motion dx and dy.
        // Update camera horizontal and vertical angle.
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;

        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        const float cameraAngularSpeed = 60.0f;

        vec3 cameraSideVector = cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));
        normalize(cameraSideVector);

        // Allows to pan the camera using the mouse movement in the x direction:
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;

            // Make sure the camera horizontal angle is between 0 and 360 degrees.
            if (cameraHorizontalAngle > 360)
                cameraHorizontalAngle -= 360;
            else if (cameraHorizontalAngle < -360)
                cameraHorizontalAngle += 360;

            float phi = radians(cameraVerticalAngle);
            float theta = radians(cameraHorizontalAngle);

            cameraLookAt = vec3((cosf(phi) * cosf(theta)), sinf(phi), (-cosf(phi) * sinf(theta)));
            cameraSideVector = cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));
        }

        // Allows to tilt the camera using the mouse movement in the y direction:
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
        {
            cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

            // Make sure the camera vertical angle is between -85.0f and 85.0f degrees.
            cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));

            float phi = radians(cameraVerticalAngle);
            float theta = radians(cameraHorizontalAngle);

            cameraLookAt = vec3((cosf(phi) * cosf(theta)), sinf(phi), (-cosf(phi) * sinf(theta)));
            cameraSideVector = cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));
        }

        // Zoom in and out:
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {

            // Make sure the field of view is between 10 and 170 degrees, otherwise we'll end up inverting the world and not getting what we really want when zooming.
            if (fov <= 10)
                fov = 10;
            else if (fov >= 170)
                fov = 170;

            fov -= dy * 0.1; // decrease the fov by 0.5

            // Update the projection matrix after zooming in or out.
            mat4 projectionMatrix = perspective(radians(fov), (800.0f / 600.0f), 0.01f, 100.0f);
            SetUniformMat4(shaderProgram, "projection_matrix", projectionMatrix);
            SetUniformMat4(lightProgram, "projection_matrix", projectionMatrix);
            SetUniformMat4(textureProgram, "projection_matrix", projectionMatrix);
        }

        // Use camera lookat and side vectors to update camera positions with arrows:
        // Moves camera to the left:
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            cameraPosition -= cameraSideVector * currentCameraSpeed * dt;

        // Moves camera to the right:
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            cameraPosition += cameraSideVector * currentCameraSpeed * dt;

        // Moves camera forward:
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            cameraPosition -= cameraLookAt * currentCameraSpeed * dt;

        // Moves camera backward:
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            cameraPosition += cameraLookAt * currentCameraSpeed * dt;

        // Restores the original camera (world) position:
        if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
        {
            cameraPosition = vec3(0.0f, 5.0f, 30.0f);
            cameraLookAt = vec3(0.0f, 0.0f, -1.0f);
            cameraUp = vec3(0.0f, 1.0f, 0.0f);
        }

        // Update the view matrix:
        viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        SetUniformMat4(shaderProgram, "view_matrix", viewMatrix);
        SetUniformMat4(lightProgram, "view_matrix", viewMatrix);
        SetUniformMat4(textureProgram, "view_matrix", viewMatrix);
    }

    // Shutdown GLFW:
    glfwTerminate();

    return 0;
}
