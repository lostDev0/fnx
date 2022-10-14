# only for cmake --version >= 3.5.1
cmake_minimum_required(VERSION 3.5.1)

# project name
project(fnx)

# set the C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# I./includes
include_directories(include)

# puts all .cpp files inside src to the SOURCES variable
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# compiles the files defined by SOURCES to generante the executable defined by EXEC
add_library(${PROJECT_NAME} STATIC ${SOURCES})

add_subdirectory(unit)
