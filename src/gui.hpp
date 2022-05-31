#pragma once
#include "iinterface.hpp"
#include <thread>
class GUI : public IInterface{
public:
    GUI();
    bool get_connect_pressed() {return true;};
    bool get_start_pressed() {return true;};
    bool get_stop_pressed() {return true;};
    void run_app();
private:

    const static int MAX_MEMORY = 4096;
    std::shared_ptr<std::thread> renderthread;
};