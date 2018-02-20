#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "rasterization.h"
#include "callbacks.h"

Framebuffer buffer;

void draw()
{
    buffer.loadBuf();
}

int main(int argc, char* argv[])
{
    // initialise GLFW
    if(!glfwInit())
    {
        printf("glfwInit failed\n");
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "A-antialiasing. F-filling. L-lined. Space-CCW. C-contrast colors. Z-cancel. E-erase", NULL, NULL);
    if (window == NULL)
    {
        printf("glfwOpenWindow failed.\n");
        glfwTerminate();
        return -2;
    }

    int attrib;
    attrib = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    attrib = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    attrib = glfwGetWindowAttrib(window, GLFW_OPENGL_PROFILE);

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetFramebufferSizeCallback(window, resize_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, cursor_callback);

    resize_callback(window, 800, 600);

    while (!glfwWindowShouldClose(window))
    {
        draw();

        glfwSwapBuffers(window);

        glfwWaitEvents();
    }

    glfwDestroyWindow(window);

    // clean up and exit
    glfwTerminate();

    return 0;
}
