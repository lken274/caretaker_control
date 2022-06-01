#pragma once
#include <asio.hpp>

class SimpleSerialOutput {
public:
    SimpleSerialOutput(std::string port, uint32_t baud_rate) : io(), serial(io,port) {
        serial.set_option(asio::serial_port_base::baud_rate(baud_rate));
    }

    void writeByte(u_char byte) {
        asio::write(serial, asio::buffer(&byte, sizeof(u_char)));
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
private:
    asio::io_service io;
    asio::serial_port serial;
};

class TriggerBox {
    public:
        TriggerBox() {
        }
        void connectToCom(std::string port) {
            ser.reset(new SimpleSerialOutput(port, 19200));
        }
        void sendTrigger(u_char trigger) {
            ser->writeByte(trigger);
            ser->writeByte(0);
        }
    private:
    std::unique_ptr<SimpleSerialOutput> ser;
};