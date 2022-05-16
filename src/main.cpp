#include <iostream>
#include "iinterface.hpp"
#include "caretakerhandler.hpp"
#include "gui.hpp"
#include "cli.hpp"
#include <cxxopts.hpp>

int main(int argc, char **argv)
{
    //program argument handling
    cxxopts::Options options("CaretakerApp", "An app for controlling the Caretaker4 platform");
    options.add_options()("h,help", "Print usage")
    ("n,nogui", "Start application in console-only mode");

    auto args = options.parse(argc, argv);
    std::shared_ptr<IInterface> io;
    if (args.count("nogui") ){
        std::cout << "Starting app in console mode" << std::endl;
        io = std::make_shared<CLI>();
    }  
    else {  
        io = std::make_shared<GUI>();
        std::cout << "Starting app in graphical mode" << std::endl;
    }

    //start Caretaker link
    CaretakerHandler cth;

    return 0;
}