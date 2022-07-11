#pragma once
#include "iinterface.hpp"
#include <thread>
#include <string>
#include "stdcapture.hpp"
class GUI : public IInterface{
public:
    GUI();
    void reset_flags() {
        conn_but_flag = false;
        start_but_flag = false;
        stop_but_flag = false;
        trigger_flag.first = false;
    }
    bool get_connect_pressed() {if (conn_but_flag) {reset_flags(); return true;} return false;};
    bool get_start_pressed() {if (start_but_flag) {reset_flags(); return true;} return false;};
    bool get_stop_pressed() {if (stop_but_flag) {reset_flags(); return true;} return false;};
    std::string get_com_port() {return std::string(com_input,com_size);};
    bool get_trigger_pressed() {if (trigger_flag.first) {reset_flags(); return true;} return false;};
    unsigned char get_trigger_value() override {return trigger_flag.second+1;};
    void run_app();
private:
    bool conn_but_flag = false;
    bool start_but_flag = false;
    bool stop_but_flag = false;
    std::pair<bool, unsigned char> trigger_flag = {false,0};
    char baud_input[64] = "192000";
    char com_input[64] = "COM7";
    int com_size = 4;
    int baud_size = 6;
    const static int MAX_MEMORY = 4096;
    std::shared_ptr<std::thread> renderthread;
    StdCapture stdcap;
    bool gui_ready = false;
};