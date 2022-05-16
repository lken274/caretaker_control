#include "caretakerhandler.hpp"
#include <iostream>

CaretakerHandler::CaretakerHandler() {
    std::cout << "Initialising Caretaker Library..." << std::endl;
    libct_init_data_t init_data;
    memset(&init_data, 0, sizeof(init_data));
    init_data.device_class = LIBCT_DEVICE_CLASS_BLE_CARETAKER4;
    libct_app_callbacks_t callbacks = {};
    libct_context_t* context = NULL;
    int status = libct_init(&context, &init_data, &callbacks);
    if ( LIBCT_FAILED(status) ) {
        std::cout << "Caretaker Library failed to initialise!" << std::endl;
    } else
    std::cout << "Caretaker Library Initialised Successfully" << std::endl;

}