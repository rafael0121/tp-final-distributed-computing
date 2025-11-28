cmake_minimum_required(VERSION 3.13)
project(tp-final-distributed-computing LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)

#
# Dependencies
#
find_package(Threads)

#
# Sources
#
set(SOURCES
    "sources/main.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

#
# Target
#
add_executable(server ${SOURCES})