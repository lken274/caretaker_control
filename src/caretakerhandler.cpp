#include "caretakerhandler.hpp"
#include <iostream>
#include <map>

#define DISCOVER_TIMEOUT 1000

void LIBCTAPI cb_on_device_discovered(libct_context_t* context, libct_device_t* device);
void LIBCTAPI cb_on_discovery_timedout(libct_context_t* context);
void LIBCTAPI cb_on_discovery_failed(libct_context_t* context, int error);
void LIBCTAPI cb_on_device_connected_ready(libct_context_t* context, libct_device_t* device);

static std::map<libct_context_t*, CaretakerHandler*> context2handler;

CaretakerHandler::CaretakerHandler(std::shared_ptr<IInterface> io) : io(io) {
    io->log("Initialising Caretaker Library...");
    memset(&hd.init_data, 0, sizeof(hd.init_data));
    hd.init_data.device_class = LIBCT_DEVICE_CLASS_BLE_CARETAKER4;
    hd.callbacks.on_device_discovered = cb_on_device_discovered;/*, cb_on_device_connected_ready, cb_on_device_disconneted, cb_on_data_received*/
    hd.callbacks.on_discovery_timedout = cb_on_discovery_timedout;
    hd.callbacks.on_discovery_failed = cb_on_discovery_failed;
    hd.callbacks.on_device_connected_ready = cb_on_device_connected_ready;
    hd.context = NULL;
    hd.status = libct_init(&hd.context, &hd.init_data, &hd.callbacks);
    context2handler[hd.context] = this;
    if ( LIBCT_FAILED(hd.status) ) {
        io->log("Caretaker Library failed to initialise! Exiting...");
        exit(1);
    } else
    io->log("Caretaker Library Initialised Successfully");

}

void CaretakerHandler::connect_to_single_device() {
    libct_start_discovery(hd.context, DISCOVER_TIMEOUT);
}

void LIBCTAPI cb_on_device_discovered(libct_context_t* context, libct_device_t* device){
    std::cout << "Successfully detected a caretaker device: " << device->get_name(device) << std::endl;
    std::cout << "Attempting to connect..." << std::endl;
    libct_stop_discovery(context);
    libct_connect(context, device);
}

void LIBCTAPI cb_on_discovery_timedout(libct_context_t* context){
    std::cout << "Could not discover any caretaker devices before specified timeout" << std::endl;
}

void LIBCTAPI cb_on_discovery_failed(libct_context_t* context, int error){
    std::cout << "Failed to search for any caretaker devices" << std::endl;
}

void LIBCTAPI cb_on_device_connected_ready(libct_context_t* context, libct_device_t* device){
    std::cout << "Successfully connected to caretaker device! " << device->get_name(device) << std::endl;
}