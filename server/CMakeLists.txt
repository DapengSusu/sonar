cmake_minimum_required(VERSION 3.5)

# Set the project name
project(sonar_system)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build/bin)

set(SOURCES
    sonar_system.c
)

# Add an executable
add_executable(sonar_system ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    lib::sonar_frame
)
