cmake_minimum_required(VERSION 3.5)

# Set the project name
project(sonar_server)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build/bin)

set(SOURCES
    server.c
)

# Add an executable
add_executable(sonar_server ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    lib::sonar_frame
)
