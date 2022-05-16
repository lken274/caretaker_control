#pragma once

#ifdef __linux__ 
    #include <caretaker.h>
#elif _WIN32
    #include <caretaker_static.h>
#else

#endif
class CaretakerHandler {

};