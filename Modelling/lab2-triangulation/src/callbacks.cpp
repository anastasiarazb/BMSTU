#include "callbacks.h"
#include "rasterization.h"

extern Framebuffer buffer;

void cursor_callback(GLFWwindow* , double, double)
{

}

void mouse_callback(GLFWwindow* window, int button, int action, int /*mods*/)
{
    if(button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if(action == GLFW_PRESS) glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        if(action == GLFW_RELEASE) glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if(button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if(action == GLFW_PRESS){
            buffer.polygon.addMousePoint(window);
        }
    }
}

void resize_callback(GLFWwindow* , int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( -(GLdouble)width, (GLdouble)width, -(GLdouble)height, (GLdouble)height, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    buffer.reinitBuffer(width, height);

}

void keyboard_callback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (action == GLFW_PRESS) {
        switch(key) //GLFW_KEY_ESCAPE GLFW_KEY_LEFT GLFW_KEY_RIGHT GLFW_KEY_SPACE
        {
        case(GLFW_KEY_C):  //C - contrast color
            buffer.polygon.contrast = !buffer.polygon.contrast;
            return;
        case(GLFW_KEY_L): //L - lined
            buffer.polygon.lined = !buffer.polygon.lined;
            return;
        case(GLFW_KEY_E): //E - erase - стереть все
            buffer.polygon.clear();
            return;
        }
    }
}

void error_callback(int /*error*/, const char* description)
{
    fputs(description, stderr);
}
