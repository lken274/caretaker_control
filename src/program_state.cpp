#include "program_state.hpp"


std::string get_name(PROGRAM_STATE state){
    switch(state){
        case IDLE: return "IDLE";break;
        case CONNECTING_CARETAKER: return "CONNECTING_CARETAKER";break;
        case CONNECTED: return "CONNECTED";break;
        case RUNNING: return "RUNNING";break;
        case QUIT: return "QUIT";break;
        default:
            return "UNKNOWN";
    }
}

static PROGRAM_STATE system_state = IDLE;
PROGRAM_STATE get_state(){
    return system_state;
}
void set_state(PROGRAM_STATE state) {
    system_state = state;
}