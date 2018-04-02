#include "callbacks.h"
#include <cstring>
#include "rasterization.h"

extern Framebuffer buffer;

void cursor_callback(GLFWwindow* , double, double)
{

}

extern const char *HEADER_MESSAGE;
void mouse_callback(GLFWwindow* window, int button, int action, int /*mods*/)
{
    if(button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if(action == GLFW_PRESS){
            double xpos, ypos;
            glfwGetCursorPos (window, &xpos, &ypos);
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            //Координаты мыши в с.к. буфера кадра
            int x = (int)floor(xpos);
            int y = height - (int)floor(ypos);
            std::string message(HEADER_MESSAGE);
            message.reserve(message.size()
                            + std::strlen(" Last mouse pose (xxx, xxx)"));
            message.append(" Last mouse pose (").append(std::to_string(x))
                    .append(", ").append(std::to_string(y)).append(")");
            glfwSetWindowTitle(window, message.c_str());
            printf("mouse at (%d, %d);\n", x, y);
        }
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


void keyboard_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (action == GLFW_PRESS) {
        switch(key) //GLFW_KEY_ESCAPE GLFW_KEY_LEFT GLFW_KEY_RIGHT GLFW_KEY_SPACE
        {
        case(GLFW_KEY_C):  //C - contrast color
            buffer.polygon.concentrate = !buffer.polygon.concentrate;
            return;
        case(GLFW_KEY_L): //L - lined
            buffer.polygon.lined = !buffer.polygon.lined;
            return;
        case(GLFW_KEY_E): //E - erase - стереть все
            buffer.polygon.clear();
            return;
        case(GLFW_KEY_Q):
        case(GLFW_KEY_ESCAPE):
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
}

void error_callback(int /*error*/, const char* description)
{
    fputs(description, stderr);
}
