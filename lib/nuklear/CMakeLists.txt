add_library(guilib INTERFACE)
target_link_libraries(guilib INTERFACE glfwlib glewlib)
target_include_directories(guilib INTERFACE .)
