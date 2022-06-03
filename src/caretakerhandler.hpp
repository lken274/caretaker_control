#pragma once

#include <caretaker_static.h>
#include "iinterface.hpp"

struct HandlerData{
    libct_init_data_t init_data;
    libct_app_callbacks_t callbacks = {};
    libct_context_t* context = NULL;
    int status;
};

class CaretakerHandler {
public:
    CaretakerHandler(std::shared_ptr<IInterface> io);
    bool connect_to_single_device();
    void start_device_readings();
    void stop_device_readings();
    bool isConnected = false;
private:
    HandlerData hd;
    std::shared_ptr<IInterface> io;
};