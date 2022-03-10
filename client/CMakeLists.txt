cmake_minimum_required(VERSION 3.5)

# Set the project name
project(pc)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build/bin)

set(SOURCES
    pc.c
)

# Add an executable
add_executable(pc ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    lib::sonar_frame
)
