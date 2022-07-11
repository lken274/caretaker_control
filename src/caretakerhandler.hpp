#pragma once

#include <caretaker_static.h>
#include "iinterface.hpp"
#include "CSVWriter.h"
#include <map>

struct DataRecord {
    unsigned long long timestamp;
    std::string data; //unused
};
struct HandlerData{
    libct_init_data_t init_data;
    libct_app_callbacks_t callbacks = {};
    libct_context_t* context = NULL;
    std::map<std::string, DataRecord> recentData; //type, data;
    bool started = false;
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
    std::shared_ptr<IInterface> io;
private:
    CSVWriter fileOut;
    std::string filename;
};