#pragma once
#include <string>

enum PROGRAM_STATE {
    IDLE,
    CONNECTING_CARETAKER,
    CONNECTED,
    RUNNING,
    QUIT
};
std::string get_name(PROGRAM_STATE state);
PROGRAM_STATE get_state();
void set_state(PROGRAM_STATE state);