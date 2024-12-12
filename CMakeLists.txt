cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD 20)

project(bitcoin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")

add_executable(bitcoin main.cpp lib/broadcast.cpp)
target_include_directories(bitcoin PUBLIC lib)

enable_testing()
