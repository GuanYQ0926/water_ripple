#include <iostream>
#include <string>

#include "glinclude.h"
#include "scene.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLuint screenWidth = 800, screenHeight = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

Camera camera(glm::vec3(0.0f, 1.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 400;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight,
                                          "ripple", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, screenWidth, screenHeight);

    glm::vec2 windowSize = glm::vec2(screenWidth, screenHeight);
    Scene* scene = new Scene();
    scene->initScene();
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Do_Movement();

        mat4 cameraView = camera.GetViewMatrix();
        mat4 cameraProjection = glm::perspective(camera.Zoom,
                                                 (float)screenWidth/(float)screenHeight,
                                                 0.1f, 100.0f);
        scene->render(cameraView, cameraProjection, lastFrame, windowSize);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}


void Do_Movement()
{
    // Camera controls
    if(keys[GLFW_KEY_W])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if(keys[GLFW_KEY_S])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if(keys[GLFW_KEY_A])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if(keys[GLFW_KEY_D])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mode)
{
    if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if(action == GLFW_PRESS)
    {
        keys[key] = true;
    }
    else if(action == GLFW_RELEASE)
    {
        keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
