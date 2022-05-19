#pragma once

#include <caretaker_static.h>

struct HandlerData{
    libct_init_data_t init_data;
    libct_app_callbacks_t callbacks = {};
    libct_context_t* context = NULL;
    int status;
};

class CaretakerHandler {
public:
    CaretakerHandler();
    void connect_to_single_device();
    bool isConnected = false;
private:
    HandlerData hd;
};