#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <mutex>
#include <ctime>
#include <iomanip>
#include "program_state.hpp"
#include <sstream>

class IInterface{
public:
    virtual bool get_connect_pressed() = 0;
    virtual bool get_start_pressed() = 0;
    virtual bool get_stop_pressed() = 0;
    virtual std::string get_com_port() = 0;
    virtual bool get_trigger_pressed() = 0;
    virtual void run_app() = 0;
    virtual unsigned char get_trigger_value() = 0;
    void log(std::string str) {
        q_mutex.lock();
        log_queue.push(str);
        q_mutex.unlock();
    };
    volatile bool running;
protected:
    std::string getLogQueue(){
        q_mutex.lock();
        std::stringstream ss;
        while(log_queue.size() > 0)
        {
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);
            ss << std::put_time(&tm, "%H:%M:%S: ") << log_queue.front() << std::endl;
            log_queue.pop();
        }
        q_mutex.unlock();
        return ss.str();
    }

    void printDate()
    {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::stringstream ss;
        ss << "Software Started: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S\n%z %Z");
        log(ss.str());
    }
    std::mutex q_mutex;
    std::queue<std::string> log_queue;
};