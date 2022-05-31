#include <iostream>
#include "iinterface.hpp"
#include "caretakerhandler.hpp"
#include "gui.hpp"
#include "cli.hpp"
#include <cxxopts.hpp>

enum PROGRAM_STATE {
    IDLE,
    CONNECTING,
    CONNECTED,
    RUNNING,
    QUIT
};


int main(int argc, char **argv)
{
    //program argument handling
    cxxopts::Options options("CaretakerApp", "An app for controlling the Caretaker4 platform");
    options.add_options()("h,help", "Print usage")
    ("n,nogui", "Start application in console-only mode");

    auto args = options.parse(argc, argv);
    std::shared_ptr<IInterface> io;
    if (args.count("nogui") ){
        std::cout << "Starting app in console mode" << std::endl;
        io = std::make_shared<CLI>();
    }  
    else {  
        io = std::make_shared<GUI>();
        std::cout << "Starting app in graphical mode" << std::endl;
    }

    CaretakerHandler cth;
    PROGRAM_STATE state = IDLE;
    bool quit = false;

    while(true);
    // while(state != QUIT){
    //     PROGRAM_STATE next_state = state;
    //     switch(state) {
    //         case IDLE:
    //             if (io->get_connect_pressed()) {
    //                 next_state = CONNECTING;
    //                 //start Caretaker link
    //                 cth.connect_to_single_device();
    //             }
    //             break;
    //         case CONNECTING:
    //             //await connection
    //             if (!cth.isConnected){
    //                 next_state = CONNECTED;
    //                 cth.start_device_readings();
    //             }
    //             break;
    //         case CONNECTED:
    //             if(io->get_start_pressed()) {
    //                 next_state = RUNNING;
    //             }
    //             break;
    //         case RUNNING:
    //             if(io->get_stop_pressed()) {
    //                 cth.stop_device_readings();
    //                 next_state = IDLE;
    //             }
    //             break;
    //     }
    //     state = next_state;
    // }

    return 0;
}