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
    while(!gui_ready){} //block until ready
}
int win_height = 480;
int win_width = 640;
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
    glfwWindowHint(GLFW_RESIZABLE, 0);
    win = glfwCreateWindow(win_width, win_height, "Caretaker Control", NULL, NULL);
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
    //fixed params
    char com_input[64] = "COM4";
    int com_size = strlen(com_input);
    char baud_input[64] = "192000";
    static const char* trigger_options[] = {"1","2","3","4","5","6","7","8","9","10"};
    int trigger_sel = 0;
    int baud_size = strlen(baud_input);
    int control_panel_width = win_width / 4;
    int control_panel_height = win_height;
    std::string console_outputs = "";
    int max_console_size = 512 * 512; //512 lines of up to 512 chars
    stdcap.BeginCapture();
    printDate();
    gui_ready = true;
     while (!glfwWindowShouldClose(win))
     {
         /* Input */
        glfwPollEvents();
        nk_glfw3_new_frame(&glfw);
         /* GUI */

        if (nk_begin(ctx, "Control", nk_rect(0, 0, control_panel_width, control_panel_height), NK_WINDOW_BORDER | NK_WINDOW_TITLE))
        {
            // nk_layout_row_dynamic(ctx, 120, 1);
            // nk_label(ctx, "Hello world!", NK_TEXT_CENTERED);
            
            // nk_layout_row_dynamic(ctx, 50, 1);
            // nk_label(ctx, "Hello world!", NK_TEXT_LEFT);
            
            nk_layout_row_static(ctx, control_panel_height / 10, control_panel_width - 40, 1);

            if (nk_button_label(ctx, "Connect"))
                log("button pressed");
            if (nk_button_label(ctx, "Start"))
                log("button pressed");

            nk_spacer(ctx);
            if (nk_button_label(ctx, "Trigger"))
                log("button pressed");
            nk_spacer(ctx);
            nk_spacer(ctx);
            nk_spacer(ctx);
            if (nk_button_label(ctx, "Stop"))
                log("button pressed");
        }
        nk_end(ctx);
        int values_panel_width = win_width - control_panel_width;
        int values_panel_height = win_height * 0.4;
        if (nk_begin(ctx, "Values", nk_rect(control_panel_width, 0, values_panel_width, values_panel_height), NK_WINDOW_BORDER | NK_WINDOW_TITLE))
        {
            nk_layout_row_dynamic(ctx, 16, 2);
            nk_label(ctx, "COM Port:", NK_TEXT_LEFT);
            nk_label(ctx, "BAUD:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 32, 2);   
            nk_edit_string(ctx, NK_EDIT_FIELD, com_input, &com_size, 64, nk_filter_default);
            nk_edit_string(ctx, NK_EDIT_FIELD, baud_input, &baud_size, 64, nk_filter_default);
            nk_layout_row_dynamic(ctx, 24, 2);
            nk_label(ctx, "BrainProducts Trigger:", NK_TEXT_LEFT);
            trigger_sel = nk_combo(ctx, trigger_options,NK_LEN(trigger_options),trigger_sel, 24, nk_vec2(200,200));
             nk_spacer(ctx);
            nk_spacer(ctx);
            nk_label(ctx, "Program Status:", NK_TEXT_LEFT);
        }
        nk_end(ctx);

        int console_panel_width = values_panel_width;
        int console_panel_height = win_height - values_panel_height;
        static const nk_flags status_flags = nk_edit_types::NK_EDIT_BOX | NK_EDIT_READ_ONLY | NK_EDIT_GOTO_END_ON_ACTIVATE;
        if (nk_begin(ctx, "Console", nk_rect(control_panel_width, values_panel_height, console_panel_width, console_panel_height), NK_WINDOW_BORDER | NK_WINDOW_TITLE))
        {
            printLogQueue();
            stdcap.EndCapture();
            std::string new_capture = stdcap.GetCapture();
            if (console_outputs.size() + new_capture.size() > max_console_size){
                console_outputs.erase(0, new_capture.size());
            }
            console_outputs += stdcap.GetCapture();
            int console_size = console_outputs.length();
            nk_layout_row_dynamic(ctx, 1000, 1);
            nk_edit_string(ctx, status_flags, (char*)console_outputs.c_str(), &console_size, max_console_size, nk_filter_default);
            stdcap.BeginCapture();
        }
        nk_end(ctx);

        if(ctx->input.mouse.buttons[NK_BUTTON_LEFT].down &&
	        ctx->input.mouse.buttons[NK_BUTTON_LEFT].clicked)
            log("Mouse clicked!");

        /* Draw */
        glViewport(0, 0, win_width, win_height);
        glClear(GL_COLOR_BUFFER_BIT);
        nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(win);
     }
    nk_glfw3_shutdown(&glfw);
    glfwTerminate();
    running = false;
}