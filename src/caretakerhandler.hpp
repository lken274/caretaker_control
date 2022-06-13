#pragma once

#include <caretaker_static.h>
#include "iinterface.hpp"
#include "CSVWriter.h"
#include <map>

struct DataRecord {
    uint32_t timestamp;
    std::string data;
};
struct HandlerData{
    libct_init_data_t init_data;
    libct_app_callbacks_t callbacks = {};
    libct_context_t* context = NULL;
    std::map<std::string, DataRecord> recentData; //type, data;
    int status;
};

class CaretakerHandler {
public:
    CaretakerHandler(std::shared_ptr<IInterface> io);
    bool connect_to_single_device();
    void start_device_readings();
    void stop_device_readings();
    void recordLastTimestamp(int triggerNum);
    bool isConnected = false;
    HandlerData hd;
private:
    CSVWriter fileOut;
    std::shared_ptr<IInterface> io;
    
};