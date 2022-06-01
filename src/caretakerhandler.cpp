#include "caretakerhandler.hpp"
#include <iostream>
#include <map>

#define DISCOVER_TIMEOUT 1000

void LIBCTAPI cb_on_device_discovered(libct_context_t* context, libct_device_t* device);
void LIBCTAPI cb_on_discovery_timedout(libct_context_t* context);
void LIBCTAPI cb_on_discovery_failed(libct_context_t* context, int error);
void LIBCTAPI cb_on_device_connected_ready(libct_context_t* context, libct_device_t* device);
void LIBCTAPI cb_on_start_measuring(libct_context_t *context, libct_device_t *device, int status);
void LIBCTAPI cb_on_data_received(libct_context_t *context, libct_device_t *device, libct_stream_data_t *data);

static std::map<libct_context_t*, CaretakerHandler*> context2handler;

CaretakerHandler::CaretakerHandler(std::shared_ptr<IInterface> io) : io(io) {
    io->log("Initialising Caretaker Library...");
    memset(&hd.init_data, 0, sizeof(hd.init_data));
    hd.init_data.device_class = LIBCT_DEVICE_CLASS_BLE_CARETAKER4;
    hd.callbacks.on_device_discovered = cb_on_device_discovered;/*, cb_on_device_connected_ready, cb_on_device_disconneted, cb_on_data_received*/
    hd.callbacks.on_discovery_timedout = cb_on_discovery_timedout;
    hd.callbacks.on_discovery_failed = cb_on_discovery_failed;
    hd.callbacks.on_device_connected_ready = cb_on_device_connected_ready;
    hd.callbacks.on_data_received = cb_on_data_received;
    hd.callbacks.on_start_measuring = cb_on_start_measuring;
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

void CaretakerHandler::start_device_readings() {
    libct_cal_t cal;
    cal.type = LIBCT_AUTO_CAL;
    cal.config.auto_cal.posture = libct_posture_t::LIBCT_POSTURE_SITTING;
    libct_start_measuring(hd.context, &cal);
}

void LIBCTAPI cb_on_start_measuring(libct_context_t *context, libct_device_t *device, int status) {
    std::cout << "Measurements started!" << std::endl;
}

void CaretakerHandler::stop_device_readings() {
    libct_stop_measuring(hd.context);
    std::cout << "Measurements stopped!" << std::endl;
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
   int flags = (LIBCT_MONITOR_INT_PULSE |
        LIBCT_MONITOR_PARAM_PULSE |
        LIBCT_MONITOR_VITALS |
        LIBCT_MONITOR_CUFF_PRESSURE |
        LIBCT_MONITOR_DEVICE_STATUS |
        LIBCT_MONITOR_BATTERY_INFO);
    libct_start_monitoring(context, flags);
    std::cout << "Successfully connected to caretaker device! " << device->get_name(device) << std::endl;
    context2handler[context]->isConnected = true;
}

void LIBCTAPI cb_on_data_received(libct_context_t *context, libct_device_t *device, libct_stream_data_t *data) {
    
}