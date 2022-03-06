cmake_minimum_required(VERSION 3.5)

# Set the project name
project(sonar_frame)

# Add a library with the above sources
add_library(${PROJECT_NAME} src/sonar.c src/tcp.c)
add_library(lib::sonar_frame ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
