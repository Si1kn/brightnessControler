cmake_minimum_required(VERSION 3.22)
project("hello")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-g")
#set(DCMAKE_BUILD_TYPE Debug)

# Must set the path to the main.cpp, for example: scripts/main.cpp if it is inside a folder
add_executable(${PROJECT_NAME} "scripts/main.cpp")
target_compile_options(${PROJECT_NAME} PRIVATE -mavx)
