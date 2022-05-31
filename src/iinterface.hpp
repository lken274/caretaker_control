#pragma once
class IInterface{
public:
    virtual bool get_connect_pressed() = 0;
    virtual bool get_start_pressed() = 0;
    virtual bool get_stop_pressed() = 0;
    virtual void run_app() = 0;
};