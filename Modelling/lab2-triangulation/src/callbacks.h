#ifndef CALLBACKS_H
#define CALLBACKS_H

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "rasterization.h"

void cursor_callback(GLFWwindow* /*window*/, double /*x*/, double /*y*/);

void mouse_callback(GLFWwindow* window, int button, int action, int /*mods*/);

void resize_callback(GLFWwindow* /*window*/, int width, int height);

void keyboard_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/);

void error_callback(int /*error*/, const char* description);


#endif // CALLBACKS_H
