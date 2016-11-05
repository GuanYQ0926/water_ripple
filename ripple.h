#ifndef RIPPLE_H
#define RIPPLE_H

#include "glinclude.h"
#include "glslprogram.h"
#include "defines.h"

#include <glm/glm.hpp>
using glm::mat4;

class Ripple
{
private:
    GLSLProgram prog;
    GLuint posBuf[2];
    GLuint normalBuf;
    GLuint texBuf, elBuf;
    GLuint leftBuf, topBuf, rightBuf, bottomBuf;
    GLuint particleArray[2];
    GLuint feedback[2];
    GLuint drawBuf;
    GLuint texID;

    int nParticles;
    int nRows;
    float nSize;
    int faces;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();
    void initBuffers();

    GLuint loadTexture(const char* file);
    GLfloat* left_points(GLfloat* data);
    GLfloat* top_points(GLfloat* data);
    GLfloat* right_points(GLfloat* data);
    GLfloat* bottom_points(GLfloat* data);

public:
    Ripple();

    void initScene();
    void render(mat4 cameraView, mat4 cameraProjection);
};

#endif // RIPPLE_H
