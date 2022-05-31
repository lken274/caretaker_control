#pragma once
#include "iinterface.hpp"
class CLI : public IInterface {
public:
    CLI();
    void init();
    bool get_connect_pressed() {return true;};
    bool get_start_pressed() {return true;};
    bool get_stop_pressed() {return true;};
    void run() {};
};