#ifndef SCENE_H
#define SCENE_H

#include "glinclude.h"
#include "glslprogram.h"
#include "defines.h"
#include "plane.h"

#include <glm/glm.hpp>
using glm::mat4;


class Scene
{
private:
    GLSLProgram prog;

    mat4 model;
    mat4 view;
    mat4 projection;
    GLuint texID;
    GLuint texID1;
    Plane* water;

    void setMatrices();
    void compileAndLinkShader();
    void loadTexture(const char* filepath);
    void loadTexture1(const char* filepath);
public:
    Scene();

    void initScene();
    void render(mat4 cameraView, mat4 cameraProjection, GLfloat time, glm::vec2 windowSize);
};

#endif // SCENE_H
