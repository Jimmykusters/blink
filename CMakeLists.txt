cmake_minimum_required(VERSION 3.10)
project(blink)

# Include directories (everything in the SDK is already included here)
set(INCLUDES
    .
    src
    src/button/inc
    )
include_directories(${INCLUDES})

# Add your source files
set(SOURCES
    src/button/button.cpp
    src/main.cpp
)

# Create the executable
add_executable(blink ${SOURCES})
install(TARGETS blink RUNTIME DESTINATION bin)