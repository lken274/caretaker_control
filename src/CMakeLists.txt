
set(SOURCE main.cpp gui.cpp caretakerhandler.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})

include_directories(
                     .
                     )

target_link_libraries(  ${CMAKE_PROJECT_NAME} 
                        cxxoptslib
                        enkilib
                        caretakerlib
                        guilib)

configure_file(appConfig.h.in appConfig.h)


