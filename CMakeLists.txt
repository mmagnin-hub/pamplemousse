cmake_minimum_required(VERSION 3.10)
project(pamplemousse DESCRIPTION "Game for Juliette")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set source encoding to UTF-8
# Ensure that source files are treated as UTF-8
add_compile_options(-finput-charset=UTF-8)  # Ensures the compiler reads files as UTF-8
add_compile_options(-fexec-charset=UTF-8)   # Ensures execution charset is UTF-8

# Find SDL2 and SDL2_image
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)


# Manually set the SDL2_image include and library paths
set(SDL2_IMAGE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/SDL2")
set(SDL2_IMAGE_LIBRARY "${CMAKE_SOURCE_DIR}/lib/SDL2_image/libSDL2_image.dylib")
set(SDL2_MIXER_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/SDL2")
set(SDL2_MIXER_LIBRARY "${CMAKE_SOURCE_DIR}/lib/SDL2_mixer/libSDL2_mixer.dylib")

# Include directories
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR})

message(STATUS "SDL2_image library: ${SDL2_IMAGE_LIBRARY}")
message(STATUS "SDL2_mixer library: ${SDL2_MIXER_LIBRARY}")

# Recursively gather all source files
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)

# Add the executable for the main application
add_executable(main ${SOURCES})

# Link SDL2, SDL2_image, and SDL2_ttf
target_link_libraries(main ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY} SDL2_ttf::SDL2_ttf)

# Ensure proper UTF-8 locale settings (for some systems like macOS)
if(APPLE)
    set(ENV{LC_ALL} "en_US.UTF-8")
    set(ENV{LANG} "en_US.UTF-8")
endif()

# Ensure proper UTF-8 font handling (if you're using SDL2_ttf)
target_compile_definitions(main PRIVATE SDL_MAIN_HANDLED)  # Necessary for SDL to handle UTF-8