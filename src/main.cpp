#define _WIN32_WINNT 0x0601
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "basic_serial.hpp"
#include "iinterface.hpp"
#include "caretakerhandler.hpp"
#include "gui.hpp"
#include <cxxopts.hpp>
#define BLE_ENABLED 0

enum PROGRAM_STATE {
    IDLE,
    CONNECTING_CARETAKER,
    CONNECTED,
    RUNNING,
    QUIT
};
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


int main(int argc, char **argv)
{
    //program argument handling
    cxxopts::Options options("CaretakerApp", "An app for controlling the Caretaker4 platform");
    options.add_options()("h,help", "Print usage")
    ("n,nogui", "Start application in console-only mode");

    auto args = options.parse(argc, argv);
    std::shared_ptr<IInterface> io;
    TriggerBox tb;
 
    io = std::make_shared<GUI>();
    io->running = true;
    std::cout << "Starting app in graphical mode" << std::endl;

    CaretakerHandler cth(io);
    PROGRAM_STATE state = IDLE;
    bool quit = false;
    
    while(state != QUIT){
        
        PROGRAM_STATE next_state = state;
        switch(state) {
            case IDLE:
                if (io->get_connect_pressed()) {
                    next_state = CONNECTING_CARETAKER;
                    //start Caretaker link
                    if(BLE_ENABLED) cth.connect_to_single_device();
                }
                break;
            case CONNECTING_CARETAKER:
                //await connection
                try {
                    tb.connectToCom(io->get_com_port());
                } catch (const std::exception& ex) {
                    io->log("Failed to connect to COM port " + std::string(io->get_com_port()));
                    std::cout << ex.what() << std::endl;
                    state = QUIT;
                    break;
                }
                if (!cth.isConnected){
                    next_state = CONNECTED;
                }
                break;
            case CONNECTED:
                if(io->get_start_pressed()) {
                    if(BLE_ENABLED) cth.start_device_readings();
                    tb.sendTrigger(io->get_trigger_value());
                    next_state = RUNNING;
                }
                break;
            case RUNNING:
                if(io->get_stop_pressed()) {
                    if(BLE_ENABLED) cth.stop_device_readings();
                    next_state = IDLE;
                }
                break;
        }
        if(state != next_state) {
           io->log("Moving state " + get_name(state) + " to " + get_name(next_state));
        }
        state = next_state;
        
        if(io->running == false) {
            state = QUIT;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20)); //prevent cpu spinning
    }

    return 0;
}