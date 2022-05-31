#include "gui.hpp"
#include <stdlib.h> 
#define NK_IMPLEMENTATION
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include <nuklear.h>
#include <nuklear_glfw_gl3.h>
#include <iostream>
static void error_callback(int e, const char *d)
{printf("Error %d: %s\n", e, d);}

GUI::GUI(){
    struct nk_context ctx;
    struct nk_glfw glfw = {0};
    static GLFWwindow *win;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) 
        {std::cout << "[GFLW] failed to init!\n" << std::endl;exit(1);}
    else{
        std::cout << "[GFLW Initialised]" << std::endl;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    win = glfwCreateWindow(800, 600, "Demo", NULL, NULL);
    glfwMakeContextCurrent(win);
}
