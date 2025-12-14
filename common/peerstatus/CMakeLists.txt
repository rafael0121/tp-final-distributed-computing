#
# Sources
#
set(SOURCES
    peer_status.cpp
)

# Create a static library from source files
add_library(peerstatus STATIC ${SOURCES})

# Optionally specify include directories
target_include_directories(peerstatus PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

