#
# Dependencies
#
find_package(Threads)

#
# Sources
#
set(SOURCES
    main.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

#
# Target
#
add_executable(compressor ${SOURCES})
target_link_libraries(compressor PRIVATE protocols)
target_link_libraries(compressor PRIVATE lamport)