add_library(guilib INTERFACE)
target_link_libraries(guilib INTERFACE glfwlib glewlib freeglutlib)
target_include_directories(guilib INTERFACE .)
