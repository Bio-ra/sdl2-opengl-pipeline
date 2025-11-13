cmake_minimum_required(VERSION 3.10)
project(OpenGL_SDL2_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(GLEW REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Include and link SDL2 and GLEW
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS})

# Add all source files
set(SOURCES
    src/main.cpp
    src/app.cpp
    src/shaderLoader.cpp
)

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${SDL2_LIBRARIES}
)

# -------------------------------
# Copy shaders to the build directory
# -------------------------------
# Collect all .glsl shader files in src/ (absolute paths)
file(GLOB SHADERS RELATIVE "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/src/*.glsl")

# Copy each shader to the executable directory after building
foreach(SHADER ${SHADERS})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/src/${SHADER}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${SHADER}"
    )
endforeach()