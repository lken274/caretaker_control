#pragma warning( disable : 4244 )
#pragma warning( disable : 4267 )
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
#include <sstream>
#include <numeric>

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
   
    int com_size = strlen(com_input);
    char baud_input[64] = "192000";
    static const char* trigger_options[] = {"1","2","3","4","5","6","7","8","9","10"};
    int trigger_sel = 0;
    int baud_size = strlen(baud_input);
    int control_panel_width = win_width / 4;
    int control_panel_height = win_height;

    static const int num_console_lines = 512;
    static const int max_text_width = 68;
    std::vector<std::string> consoleOutput;
    consoleOutput.reserve(num_console_lines);
    static const int max_console_size = num_console_lines*128;
    std::string consoleBuff;
    
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
            
            nk_layout_row_dynamic(ctx, control_panel_height / 10, 1);

            if (nk_button_label(ctx, "Connect"))
                conn_but_flag = true;
            if (nk_button_label(ctx, "Start"))
                start_but_flag = true;

            nk_spacer(ctx);
            if (nk_button_label(ctx, "Trigger"))
                trigger_flag.first = true;
            nk_spacer(ctx);
            nk_spacer(ctx);
            nk_spacer(ctx);
            if (nk_button_label(ctx, "Stop"))
                stop_but_flag = true;
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
            trigger_flag.second = nk_combo(ctx, trigger_options,NK_LEN(trigger_options),trigger_flag.second, 24, nk_vec2(200,200));
            nk_spacer(ctx);
            nk_spacer(ctx);
            nk_label(ctx, "Program State:", NK_TEXT_LEFT);
            nk_label(ctx, get_name(get_state()).c_str(), NK_TEXT_LEFT);
        }
        nk_end(ctx);

        int console_panel_width = values_panel_width;
        int console_panel_height = win_height - values_panel_height;
        static const nk_flags status_flags = nk_edit_types::NK_EDIT_EDITOR | NK_EDIT_SELECTABLE | NK_EDIT_MULTILINE;
        struct nk_vec2 orig_padding = ctx->style.window.padding;
        ctx->style.window.padding = nk_vec2(0, 0);
        if (nk_begin(ctx, "Console", nk_rect(control_panel_width, values_panel_height, console_panel_width, console_panel_height)
            , NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR))
        {
            stdcap.BeginCapture();
            printLogQueue();
            stdcap.EndCapture();
            
            std::string new_capture = stdcap.GetCapture();
            std::vector<std::string> splitVals;
            auto ss = std::stringstream{new_capture};
            for (std::string line; std::getline(ss, line, '\n');) {

                while (line.size() > max_text_width) {
                    splitVals.push_back(line.substr(0,max_text_width) + "\n");
                    line = line.substr(max_text_width, line.size());
                }
                splitVals.push_back(line + "\n");
            }
            int new_size = consoleOutput.size() + splitVals.size();
            if (new_size > num_console_lines) {
                consoleOutput.erase(consoleOutput.begin(), consoleOutput.begin() + (new_size-num_console_lines)+1);
            }
            consoleOutput.insert( consoleOutput.end(), splitVals.begin(), splitVals.end() );
            
            consoleBuff = std::accumulate(consoleOutput.begin(), consoleOutput.end(), std::string(""));
            int console_size = consoleBuff.length();
            nk_layout_row_dynamic(ctx, console_panel_height-25, 1);
            nk_edit_focus(ctx,0);
            nk_edit_string(ctx, status_flags, (char*)(consoleBuff.c_str()), &console_size, max_console_size, nk_filter_default);
        }
        nk_end(ctx);
        ctx->style.window.padding = orig_padding;
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