cmake_minimum_required(VERSION 3.16)

# project name
project(fnx)

set(HEADER_FILES include/fnx/fnx.hpp)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# set the C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-G" "_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
add_compile_definitions(_FNX_WINDOW GLEW_STATIC _CRT_SECURE_NO_WARNINGS GLEW_NO_GLU GLFW_INCLUDE_NONE)

# I./includes
include_directories(include dependencies/include /usr/include/sdl)
link_directories(dependencies/lib)

# puts all .cpp files inside src to the SOURCES variable
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# compiles the files defined by SOURCES to generante the executable defined by EXEC
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_precompile_headers(${PROJECT_NAME} PRIVATE ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME} reactphysics3d.lib opengl32.lib glew32s.lib glfw3.lib)

add_subdirectory(unit)
add_subdirectory(helloworld)
add_subdirectory(sandbox)
