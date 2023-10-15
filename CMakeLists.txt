cmake_minimum_required(VERSION 3.27)
project(polycat VERSION 0.1.0 LANGUAGES CXX)

set(SOURCES
    src/cpu.cpp
    src/args.cpp
    src/conf.cpp
    src/framer.cpp
    src/polycat.cpp
    src/rate_poll.cpp
)

add_executable(polycat ${SOURCES})
target_include_directories(polycat PRIVATE src/include)
target_compile_options(polycat PRIVATE 
    -Wall 
    -Wpedantic 
    -Werror 
    -Wno-sign-compare)
set_property(TARGET polycat PROPERTY CXX_STANDARD 20)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(dep/nlohmann-json)

add_subdirectory(dep/argparse)

target_link_libraries(polycat PRIVATE nlohmann_json::nlohmann_json argparse)
