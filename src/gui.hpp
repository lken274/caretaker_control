#pragma once
#include "iinterface.hpp"
class GUI : public IInterface{
public:
    GUI();
    bool get_connect_pressed() {return true;};
    bool get_start_pressed() {return true;};
    bool get_stop_pressed() {return true;};
    void run() {};
private:

    const static int MAX_MEMORY = 4096;
};