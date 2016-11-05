#include "scene.h"
#include <soil/SOIL.h>

using glm::vec3;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <stdio.h>

Scene::Scene()
{
}

void Scene::initScene()
{
    compileAndLinkShader();

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    projection = mat4(1.0f);
    model = mat4(1.0f);
    loadTexture("/home/guanyuqing/Documents/cppcode/waterripple/waterripple/textures/wall.jpg");
    loadTexture1("/home/guanyuqing/Documents/cppcode/waterripple/waterripple/textures/water.jpg");

    water = new Plane(15.0f, 15.0f, 300, 300);
}

void Scene::render(mat4 cameraView, mat4 cameraProjection, GLfloat time, glm::vec2 windowSize)
{
    glClear(GL_COLOR_BUFFER_BIT);
    prog.setUniform("Time", time);
    //prog.setUniform("Resolution", windowSize);
    view = cameraView;
    projection = cameraProjection;
    setMatrices();

    //glBindTexture(GL_TEXTURE_2D, texID);
    //glBindTexture(GL_TEXTURE_2D, texID1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glUniform1i(glGetUniformLocation(prog.getHandle(), "bottomTexture"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texID1);
    glUniform1i(glGetUniformLocation(prog.getHandle(), "waterTexture"), 1);

    water->render();
}

void Scene::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("MVP", projection*mv);
}

void Scene::compileAndLinkShader()
{
    if(!prog.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/waterripple/waterripple/shaders/ripple.vs", GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if(!prog.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/waterripple/waterripple/shaders/ripples.fs", GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if(!prog.link())
    {
        printf("Shader program failed to link!\n%s",
               prog.log().c_str());
        exit(1);
    }
    prog.use();
}

void Scene::loadTexture(const char* filepath)
{
    int width, height;
    unsigned char* image = SOIL_load_image(filepath, &width, &height, 0, SOIL_LOAD_RGB);
    //glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
}

void Scene::loadTexture1(const char* filepath)
{
    int width, height;
    unsigned char* image = SOIL_load_image(filepath, &width, &height, 0, SOIL_LOAD_RGB);
    //glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &texID1);
    glBindTexture(GL_TEXTURE_2D, texID1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
}



































































