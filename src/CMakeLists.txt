
set(SOURCE main.cpp gui.cpp cli.cpp caretakerhandler.cpp)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(BUILD_SHARED_LIBS TRUE)

add_executable(${PROJECT_NAME} ${SOURCE})

include_directories(
                     .
                     "${CMAKE_SOURCE_DIR}/lib/caretakerlib/"
                     )

target_link_libraries(  ${CMAKE_PROJECT_NAME} 
                        cxxoptslib
                        enkilib
                        guilib
                        setupapi.lib
                        "${CMAKE_SOURCE_DIR}/lib/caretakerlib/Win64/libcaretaker_static.lib"
                        )


configure_file(appConfig.h.in appConfig.h)


