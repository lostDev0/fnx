# only for cmake --version >= 3.5.1
cmake_minimum_required(VERSION 3.5.1)

# project name
project(fnx-hello-world)

# I../includes
include_directories(../include ../)

# puts all .cpp files inside src to the SOURCES variable
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

# compiles the files defined by SOURCES to generante the executable defined
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} fnx)