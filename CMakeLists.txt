cmake_minimum_required(VERSION 3.13)
project(tp-final-distributed-computing LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)

# Add libraries
add_subdirectory(common/lamport)
add_subdirectory(common/protocols)
add_subdirectory(common/peerstatus)
add_subdirectory(compressor)
add_subdirectory(sensor)