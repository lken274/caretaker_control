add_library(jsonlib INTERFACE)
target_include_directories(jsonlib INTERFACE .)
target_link_libraries(jsonlib INTERFACE pegtllib)