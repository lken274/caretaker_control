add_subdirectory(cxxopts)
add_subdirectory(doctest)
add_subdirectory(enkiTS)
add_subdirectory(taojson)

if (WIN32)
    add_subdirectory(windows)
endif (WIN32)

if (UNIX)
    add_subdirectory(linux)
endif (UNIX)