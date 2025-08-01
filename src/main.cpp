// clang-format off
#include <glad/glad.h>
// clang-format on
#include "Game.hpp"

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

float deltaTime, lastFrame = 0.0f;

float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
Game game(WINDOW_WIDTH, WINDOW_HEIGHT);

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    game.getCamera()->ProcessMouseMovement(xoffset, yoffset);
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window =
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "hypickel", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    if (!window)
    {
        std::cerr << "roweferwfc";
        exit(EXIT_FAILURE);
    }

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "l bozo icl fr fr";
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(window,
                                   [](GLFWwindow* window, int width, int height)
                                   {
                                       glViewport(0, 0, width, height);
                                       WINDOW_WIDTH = width;
                                       WINDOW_HEIGHT = height;

                                       game.handleResize(width, height);
                                   });
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    game.Init(window);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        game.Update(deltaTime);
        game.handleInput(window, deltaTime);

        glClearColor(0.141, 0.549, 0.894, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        game.Render();

        glfwSwapBuffers(window);
    }
}
