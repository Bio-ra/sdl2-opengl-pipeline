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
    src/quad.cpp
    src/textureHandler.cpp
    src/shaderUniformHandler.cpp
    src/transformation.cpp
    src/camera.cpp
    src/sprite.cpp
    src/spriteBatch.cpp
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