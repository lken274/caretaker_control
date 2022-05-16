include_directories (${TEST_SOURCE_DIR}/src)

add_executable (Test doctest.cpp)
target_link_libraries (Test
                       doctestlib
                       )