file(GLOB ENKY RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")
add_library(enkilib ${ENKY})
target_include_directories(enkilib INTERFACE . ${CMAKE_CURRENT_SOURCE_DIR})