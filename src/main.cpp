#define _WIN32_WINNT 0x0601
#include <iostream>
#include "basic_serial.hpp"
#include "iinterface.hpp"
#include "caretakerhandler.hpp"
#include "gui.hpp"
#include <cxxopts.hpp>
#include "program_state.hpp"
#define USB_ENABLED 1

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
    bool quit = false;
    set_state(IDLE);
    
    while(get_state() != QUIT){
        
        PROGRAM_STATE next_state = get_state();
        switch(get_state()) {
            case IDLE:
                if (io->get_connect_pressed()) {
                    bool didConnectEEG = tb.connectToCom(io->get_com_port());
                    if (!didConnectEEG) {
                        io->log("Failed to connect to COM port " + io->get_com_port());
                        break;
                    }
                    io->log("Connected to EEG COM port on " + io->get_com_port());
                    //start Caretaker link
                    if(USB_ENABLED) {
                        bool discovery_started = cth.connect_to_single_device();
                        if(!discovery_started) {
                            io->log("Failed to begin Caretaker discovery (check usb/bluetooth)");
                            tb.endComConnection();
                            break;
                        }
                    }
                    next_state = CONNECTING_CARETAKER;
                }
                break;
            case CONNECTING_CARETAKER:
                //await connection
                if (cth.isConnected || USB_ENABLED == 0){
                    next_state = CONNECTED;
                }
                break;
            case CONNECTED:
                if(io->get_start_pressed()) {
                    if(USB_ENABLED) cth.start_device_readings();
                    tb.sendTrigger(io->get_trigger_value());
                    io->log("Send trigger " + std::to_string((int)io->get_trigger_value()));
                    next_state = RUNNING;
                }
                if(io->get_stop_pressed()) {
                    if(USB_ENABLED) cth.stop_device_readings();
                    tb.endComConnection();
                    next_state = IDLE;
                }
                break;
            case RUNNING:
                if(io->get_trigger_pressed()) {
                    tb.sendTrigger(io->get_trigger_value());
                    io->log("Sent trigger " + std::to_string((int)io->get_trigger_value()));
                    cth.recordLastTimestamp(io->get_trigger_value());
                }
                
                break;
        }
        //common logic
        //
        if(io->get_stop_pressed()) {
            if(USB_ENABLED) cth.stop_device_readings();
            tb.endComConnection();
            next_state = IDLE;
        }
        /////////////////////////////
        if(get_state() != next_state) {
           io->log("Moving state " + get_name(get_state()) + " to " + get_name(next_state));
        }
        set_state(next_state);
        
        if(io->running == false) {
            set_state(QUIT);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20)); //prevent cpu spinning
    }

    return 0;
}