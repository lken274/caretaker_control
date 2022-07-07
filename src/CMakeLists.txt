
set(SOURCE main.cpp gui.cpp caretakerhandler.cpp stdcapture.cpp program_state.cpp)
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
                        freeglutlib
                        glewlib
                        glfwlib
                        setupapi.lib
                        asiolib
                        "${CMAKE_SOURCE_DIR}/lib/caretakerlib/Win64/libcaretaker_static.lib"
                        "${CMAKE_SOURCE_DIR}/lib/glew_lib/lib/Release/x64/glew32.lib"
                        "${CMAKE_SOURCE_DIR}/lib/glfw_lib/lib-vc2017/glfw3.lib"
                        "${CMAKE_SOURCE_DIR}/lib/freeglut_lib/lib/x64/freeglut.lib"
                        )

add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different    
    ${PROJECT_SOURCE_DIR}/lib/glew_lib/bin/Release/x64/glew32.dll
    ${PROJECT_SOURCE_DIR}/lib/freeglut_lib/bin/x64/freeglut.dll
    $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)
    
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
configure_file(appConfig.h.in appConfig.h)


