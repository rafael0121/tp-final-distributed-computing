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
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

#
# Target
#
add_executable(node ${SOURCES})
target_link_libraries(node PRIVATE ringElection)
target_link_libraries(node PRIVATE discoverNodes)
target_link_libraries(node PRIVATE lamport)
target_link_libraries(node PRIVATE peerstatus)
target_link_libraries(node PRIVATE logger)
target_link_libraries(node PRIVATE utils)