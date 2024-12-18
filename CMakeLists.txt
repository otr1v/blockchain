cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD 20)

project(blockchain)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")

add_executable(blockchain main.cpp lib/broadcast.cpp)
target_include_directories(blockchain PUBLIC lib)
target_link_options(blockchain PRIVATE -Wl,--gc-sections)

enable_testing()

install(TARGETS blockchain DESTINATION bin)
