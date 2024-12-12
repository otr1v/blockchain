cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD 20)

project(blockchain)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")

add_executable(blockchain main.cpp lib/broadcast.cpp)
target_include_directories(blockchain PUBLIC lib)

enable_testing()

install(TARGETS blockchain DESTINATION bin)
