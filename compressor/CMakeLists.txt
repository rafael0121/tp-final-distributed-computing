#
# Dependencies
#
find_package(Threads)

#
# Sources
#
set(SOURCES
    main.cpp
    rpc_election.cpp
    rpc_discover.cpp
    rpc_compressor.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

#
# Target
#
add_executable(compressor ${SOURCES})
target_link_libraries(compressor PRIVATE ringElection)
target_link_libraries(compressor PRIVATE discoverNodes)
target_link_libraries(compressor PRIVATE compressorNode)
target_link_libraries(compressor PRIVATE lamport)