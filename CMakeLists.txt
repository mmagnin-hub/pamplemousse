cmake_minimum_required(VERSION 3.10)
project(pamplemousse
    DESCRIPTION "Game for Juliette"
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SDL2 and SDL2_image
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

# Recursively gather all source files
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# Add the executable for the main application
add_executable(main ${SOURCES})

# Link SDL2 and SDL2_image
target_link_libraries(main ${SDL2_LIBRARIES} SDL2_image::SDL2_image)
