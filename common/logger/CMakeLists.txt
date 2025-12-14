#
# Sources
#
set(SOURCES
    logger.cpp
)

# Create a static library from source files
add_library(logger STATIC ${SOURCES})

# Optionally specify include directories
target_include_directories(logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

