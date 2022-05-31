#include "gui.hpp"
#include <stdlib.h> 
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_IMPLEMENTATION
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include <nuklear.h>
#include <nuklear_glfw_gl3.h>
#include <GL/glut.h>
#include <iostream>
static void error_callback(int e, const char *d)
{printf("Error %d: %s\n", e, d);}

GUI::GUI(){
    renderthread = std::make_shared<std::thread>([this]{run_app();});

}
int win_height = 600;
int win_width = 800;
void GUI::run_app(){
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
    win = glfwCreateWindow(win_width, win_height, "Demo", NULL, NULL);
    glfwMakeContextCurrent(win);

    /* Glew */
    glewExperimental = 1;
    if (glewInit() != GLEW_OK) 
    {std::cout << "[GLEW] failed to init!\n" << std::endl;exit(1);}
    else{
        std::cout << "[GLEW Initialised]" << std::endl;
    }
    /* create context */
    struct nk_context *ctx = nk_glfw3_init(&glfw, win, NK_GLFW3_INSTALL_CALLBACKS);

    {struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&glfw, &atlas);
    nk_glfw3_font_stash_end(&glfw);}

     while (!glfwWindowShouldClose(win))
     {
         /* Input */
        glfwPollEvents();
        nk_glfw3_new_frame(&glfw);
         /* GUI */
        if (nk_begin(ctx, "Anything window", nk_rect(20, 20, 200, 300), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
        {
            nk_layout_row_dynamic(ctx, 120, 1);
            nk_label(ctx, "Hello world!", NK_TEXT_CENTERED);
            
            nk_layout_row_dynamic(ctx, 50, 1);
            nk_label(ctx, "Hello world!", NK_TEXT_LEFT);
            
            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_button_label(ctx, "AnyButton"))
                fprintf(stdout, "button pressed\n");
        }
        nk_end(ctx);
        
        if (nk_begin(ctx, "Copy/Paste is fun", nk_rect(300, 20, 200, 300), NK_WINDOW_TITLE))
        {
            nk_layout_row_dynamic(ctx, 120, 1);
            nk_label(ctx, "Hello world!", NK_TEXT_CENTERED);
            
            nk_layout_row_dynamic(ctx, 50, 1);
            nk_label(ctx, "Hello world!", NK_TEXT_LEFT);
            
            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_button_label(ctx, "AnyButton"))
                fprintf(stdout, "button pressed\n");
        }
        nk_end(ctx);

        /* Draw */
        glViewport(0, 0, win_width, win_height);
        glClear(GL_COLOR_BUFFER_BIT);
        nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(win);
     }

}