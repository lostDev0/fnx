cmake_minimum_required(VERSION 3.16)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif(CCACHE_FOUND)

# project name
project(fnx)

# TODO headless build

set(HEADER_FILES include/fnx/fnx.hpp)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# set the C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(MSVC)
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
else()
    set(CMAKE_CXX_FLAGS "-w")
    set(CMAKE_CXX_FLAGS_DEBUG "-G" "_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

add_compile_definitions(YAML_BUILD_SHARED_LIBS=OFF)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_definitions(GLEW_STATIC _CRT_SECURE_NO_WARNINGS GLEW_NO_GLU GLFW_INCLUDE_NONE)
else()
    add_compile_definitions(_FNX_WINDOW GLEW_STATIC _CRT_SECURE_NO_WARNINGS GLEW_NO_GLU GLFW_INCLUDE_NONE)
endif()

# include dependencies and project files
set(INC_DEPENDENCIES /usr/include/sdl dependencies/include dependencies/yaml-cpp/include)
include_directories(include ${INC_DEPENDENCIES})

# puts all .cpp files inside src to the SOURCES variable
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# compiles the files defined by SOURCES to generante the executable defined by EXEC
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_precompile_headers(${PROJECT_NAME} PRIVATE ${HEADER_FILES})

# linking dependencies
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    link_directories(dependencies/lib/linux)
    set(LIB_DEPENDENCIES libreachphysics3d.a opengl32.lib glew32s.lib glfw3.lib)
else()
    link_directories(dependencies/lib/win32)
    set(LIB_DEPENDENCIES reactphysics3d.lib opengl32.lib glew32s.lib glfw3.lib)
endif()

target_link_libraries(${PROJECT_NAME} yaml-cpp)
target_link_libraries(${PROJECT_NAME} ${LIB_DEPENDENCIES})

add_subdirectory(dependencies/yaml-cpp)
add_subdirectory(unit)
add_subdirectory(helloworld)
add_subdirectory(sandbox)