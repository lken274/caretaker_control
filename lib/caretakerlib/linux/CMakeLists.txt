#untested
add_library(caretakerlib INTERFACE)
target_include_directories(caretakerlib INTERFACE .)
target_link_libraries(caretakerlib INTERFACE "${PROJECT_SOURCE_DIR}/lib/caretakerlib/linux/libcaretaker.so")
