#pragma once
#include "iinterface.hpp"
#include <thread>
#include <string>
#include "stdcapture.hpp"
class GUI : public IInterface{
public:
    GUI();
    bool get_connect_pressed() {return true;};
    bool get_start_pressed() {return true;};
    bool get_stop_pressed() {return false;};
    std::string get_com_port() {return "COM1";};
    unsigned char get_trigger_value() override {return 1;};
    void run_app();
private:

    const static int MAX_MEMORY = 4096;
    std::shared_ptr<std::thread> renderthread;
    StdCapture stdcap;
    bool gui_ready = false;
};