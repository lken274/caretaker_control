#pragma once
#include <asio.hpp>
#include <iostream>
#include <string>
class SimpleSerialOutput {
public:
    SimpleSerialOutput(std::string port, uint32_t baud_rate) : io(), serial(io,port) {
        serial.set_option(asio::serial_port_base::baud_rate(baud_rate));
    }

    void writeByte(u_char byte) {
        serial.write_some(asio::buffer(&byte, 1));
    }

    std::string readLine() {
        char c;
        std::string result;
        for(;;)
        {
            asio::read(serial,asio::buffer(&c,1));
            switch(c)
            {
                case '\r':
                    break;
                case '\n':
                    return result;
                default:
                    result+=c;
            }
        }
    }

    u_char readByte() {
        u_char c;
        asio::read(serial,asio::buffer(&c,1));
        return c;
    }

    void closePort(){
        serial.close();
    }
private:
    asio::io_service io;
    asio::serial_port serial;
};

class TriggerBox {
    public:
        TriggerBox() {
        }
        bool connectToCom(std::string port) {
            try{
                ser.reset(new SimpleSerialOutput(port, 19200));
                return true;
            } catch (const std::exception& e) {
                return false;
            }
        }
        void sendTrigger(u_char trigger) {
            ser->writeByte(trigger);
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
            ser->writeByte(0x00);
        }
        void endComConnection(){
            if(ser)
                ser->closePort();
        }
    private:
    std::unique_ptr<SimpleSerialOutput> ser;
};