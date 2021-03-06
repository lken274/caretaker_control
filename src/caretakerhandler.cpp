#include "caretakerhandler.hpp"
#include <iostream>
#include <map>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <chrono>
#define DISCOVER_TIMEOUT 10000

void LIBCTAPI cb_on_device_discovered(libct_context_t* context, libct_device_t* device);
void LIBCTAPI cb_on_discovery_timedout(libct_context_t* context);
void LIBCTAPI cb_on_discovery_failed(libct_context_t* context, int error);
void LIBCTAPI cb_on_device_connected_ready(libct_context_t* context, libct_device_t* device);
void LIBCTAPI cb_on_start_measuring(libct_context_t *context, libct_device_t *device, int status);
void LIBCTAPI cb_on_data_received(libct_context_t *context, libct_device_t *device, libct_stream_data_t *data);
void LIBCTAPI cb_on_start_monitoring(libct_context_t *context, libct_device_t *device, int status);

std::string GetCurrentTimeForFileName()
{
    auto time = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%F_%T") << ".csv"; // ISO 8601 without timezone information.
    auto s = ss.str();
    std::replace(s.begin(), s.end(), ':', '-');
    return s;
}

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

CaretakerHandler::CaretakerHandler(std::shared_ptr<IInterface> io) : io(io), fileOut(",",5) /*trigger, label, value, timestamp, computer timestamp*/ {
    io->log("Initialising Caretaker Library...");
    memset(&hd.init_data, 0, sizeof(hd.init_data));
    hd.init_data.device_class = LIBCT_DEVICE_CLASS_USB;
    hd.callbacks.on_device_discovered = cb_on_device_discovered;/*, cb_on_device_connected_ready, cb_on_device_disconneted, cb_on_data_received*/
    hd.callbacks.on_discovery_timedout = cb_on_discovery_timedout;
    hd.callbacks.on_discovery_failed = cb_on_discovery_failed;
    hd.callbacks.on_device_connected_ready = cb_on_device_connected_ready;
    hd.callbacks.on_data_received = cb_on_data_received;
    hd.callbacks.on_start_measuring = cb_on_start_measuring;
    hd.callbacks.on_start_monitoring = cb_on_start_monitoring;
    hd.context = NULL;
    hd.status = libct_init(&hd.context, &hd.init_data, &hd.callbacks);
    libct_set_app_specific_data(hd.context, this);
    if ( LIBCT_FAILED(hd.status) ) {
        io->log("Caretaker Library failed to initialise! Exiting...");
        exit(1);
    } else
    io->log("Caretaker Library Initialised Successfully");
    filename = GetCurrentTimeForFileName();
    fileOut << "trigger" << "datatype" << "recent value" << "ct timestamp" << "computer timestamp";
    fileOut.writeToFile(filename);
}

bool CaretakerHandler::connect_to_single_device() {
    int err = libct_start_discovery(hd.context, DISCOVER_TIMEOUT);
    if (err == LIBCT_COMMON_STATUS_ERROR) {
        return false;
    } return true;
}

void CaretakerHandler::start_device_readings() {
    libct_cal_t cal;
    cal.type = LIBCT_AUTO_CAL;
    cal.config.auto_cal.posture = libct_posture_t::LIBCT_POSTURE_SITTING;
    libct_start_measuring(hd.context, &cal);
}

void CaretakerHandler::stop_device_readings() {
    libct_stop_measuring(hd.context);
    libct_stop_monitoring(hd.context);
    io->log("Measurements stopped!");
}

void CaretakerHandler::recordLastTimestamp(int triggerNum) {
    std::time_t localTime = std::time(nullptr);
    for (auto& datatype : hd.recentData) {
        fileOut << triggerNum << datatype.first << datatype.second.data << datatype.second.timestamp << timeSinceEpochMillisec();
    }
    std::cout << "Writing " << hd.recentData.size() << " data readings to file" << std::endl;
    fileOut.writeToFile(filename);
}
///CALLBACKS///

void LIBCTAPI cb_on_start_measuring(libct_context_t *context, libct_device_t *device, int status) {
    CaretakerHandler* handler = (CaretakerHandler*) libct_get_app_specific_data(context);
    handler->hd.started = true;
    handler->io->log("Measurements started!");
}

void LIBCTAPI cb_on_device_discovered(libct_context_t* context, libct_device_t* device){
    CaretakerHandler* handler = (CaretakerHandler*) libct_get_app_specific_data(context);
    handler->io->log("Successfully detected a caretaker device: " + std::string(device->get_name(device)));
    handler->io->log("Attempting to connect...");
    libct_stop_discovery(context);
    libct_connect(context, device);
}

void LIBCTAPI cb_on_discovery_timedout(libct_context_t* context){
    CaretakerHandler* handler = (CaretakerHandler*) libct_get_app_specific_data(context);
    handler->io->log("Could not discover any caretaker devices before timeout");
}

void LIBCTAPI cb_on_discovery_failed(libct_context_t* context, int error){
    CaretakerHandler* handler = (CaretakerHandler*) libct_get_app_specific_data(context);
    handler->io->log("Failed to search for any caretaker devices");
}

void LIBCTAPI cb_on_device_connected_ready(libct_context_t* context, libct_device_t* device){
   int flags = (LIBCT_MONITOR_INT_PULSE |
                LIBCT_MONITOR_VITALS |
                LIBCT_MONITOR_VITALS2 |
                LIBCT_MONITOR_CUFF_PRESSURE |
                LIBCT_MONITOR_DEVICE_STATUS);

    
    libct_start_monitoring(context, flags);
    CaretakerHandler* handler = (CaretakerHandler*) libct_get_app_specific_data(context);
    handler->io->log("Successfully connected to caretaker device! " + std::string(device->get_name(device)));
    handler->isConnected = true;
}

void LIBCTAPI cb_on_start_monitoring(libct_context_t *context, libct_device_t *device, int status) {
    CaretakerHandler* handler = (CaretakerHandler*) libct_get_app_specific_data(context);
    if (status == 0) {
        handler->io->log("Device monitoring starting successfully");
    }
    else handler->io->log("Device monitoring failed to start!");
}

void LIBCTAPI cb_on_data_received(libct_context_t *context, libct_device_t *device, libct_stream_data_t *data) {
    CaretakerHandler* handler = (CaretakerHandler*) libct_get_app_specific_data(context);
    if (handler == 0) throw std::runtime_error(std::string("Couldn't find handler"));
    if (handler->hd.started == false) return;

    if (data->int_pulse.count > 0) {
        handler->hd.recentData["int pulse"].timestamp = (unsigned long long) data->int_pulse.timestamps[data->int_pulse.count-1];
        handler->hd.recentData["int pulse"].data = std::to_string(data->int_pulse.samples[data->int_pulse.count-1]);
    }
    if (data->device_status.valid) {
        handler->hd.recentData["status"].timestamp = (unsigned long long) data->device_status.timestamp;
        handler->hd.recentData["status"].data = "n/a";
    }
    if (data->cuff_pressure.count > 0) {
        handler->hd.recentData["cuff"].timestamp =  (unsigned long long) data->cuff_pressure.datapoints[data->cuff_pressure.count-1].timestamp;
        handler->hd.recentData["cuff"].data = std::to_string(data->cuff_pressure.datapoints[data->cuff_pressure.count-1].value);
    }
    if (data->vitals.count > 0) {
        int num_vals = data->vitals.count;
        unsigned long long timestamp = (unsigned long long) data->vitals.datapoints[num_vals-1].timestamp;
        handler->hd.recentData["systolic"].timestamp = timestamp;
        handler->hd.recentData["systolic"].data = std::to_string(data->vitals.datapoints[num_vals-1].systolic);
        handler->hd.recentData["diastolic"].timestamp = timestamp;
        handler->hd.recentData["diastolic"].data = std::to_string(data->vitals.datapoints[num_vals-1].diastolic);
        handler->hd.recentData["heart_rate"].timestamp = timestamp;
        handler->hd.recentData["heart_rate"].data = std::to_string(data->vitals.datapoints[num_vals-1].heart_rate);
        handler->hd.recentData["map"].timestamp = timestamp;
        handler->hd.recentData["map"].data = std::to_string(data->vitals.datapoints[num_vals-1].map);
        handler->hd.recentData["respiration"].timestamp = timestamp;
        handler->hd.recentData["respiration"].data = std::to_string(data->vitals.datapoints[num_vals-1].respiration);
    }
    if (data->vitals2.count > 0) {
        int num_vals = data->vitals2.count;
        unsigned long long timestamp = (unsigned long long) data->vitals2.datapoints[num_vals-1].timestamp;
        handler->hd.recentData["stroke_volume"].timestamp = timestamp;
        handler->hd.recentData["stroke_volume"].data = std::to_string(data->vitals2.datapoints[num_vals-1].strokeVolume);
        handler->hd.recentData["cardiac_output"].timestamp = timestamp;
        handler->hd.recentData["cardiac_output"].data = std::to_string(data->vitals2.datapoints[num_vals-1].cardiac_output);
    }
    
}