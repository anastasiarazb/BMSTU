#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "rasterization.h"
#include "callbacks.h"

Framebuffer buffer("output.txt");

void draw()
{
    buffer.loadBuf();
}

int main()
{
    // initialise GLFW
    if(!glfwInit())
    {
        printf("glfwInit failed\n");
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "L-lined. C-contrast colors. E-erase. Q/Esc-close. RBM-show coords.", NULL, NULL);
    if (window == NULL)
    {
        printf("glfwOpenWindow failed.\n");
        glfwTerminate();
        return -2;
    }

//    int attrib;
//    attrib = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
//    attrib = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
//    attrib = glfwGetWindowAttrib(window, GLFW_OPENGL_PROFILE);

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetFramebufferSizeCallback(window, resize_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
//    glfwSetCursorPosCallback(window, cursor_callback);

    resize_callback(window, 800, 600);
//int counter = 0;
    while (!glfwWindowShouldClose(window))
    {
        draw();

        glfwSwapBuffers(window);
//        printf( "%i\n", ++counter );

        glfwWaitEvents();
    }

    glfwDestroyWindow(window);

    // clean up and exit
    glfwTerminate();

    return 0;
}

/*
Дана нерегулярная сетка, сформированная 10-ю точками, иллюстрирующая
показатель прироста населения (в процентах)  в населенных пунктах
Воронежской области за прошедщий год.

Свернуть
x        y       z
10.95    0.37        0
9.72      5.12   4.11
9.24      5.32   5.24
7.33      5.35   6.15
7.50      6.13   7.16
5.62      0.29   5.27
6.75      0.31   5.49
7.10      0.82   4.53
7.05      1.97   3.12
10.03    1.81   2.15
  Провести сгущение сети (не менее 100 точек).
*/
