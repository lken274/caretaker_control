include_directories (${TEST_SOURCE_DIR}/src)

add_executable (RunTests doctest.cpp)
target_link_libraries (RunTests
                       doctestlib
                       )