#pragma once

#include <format>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>

inline std::string get_current_time() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm *local_time = std::localtime(&now_time);

    std::stringstream time_stream;
    time_stream << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");

    return time_stream.str();
}

#ifndef NOLOG
#define LOG(fmt, ...) ((void) (std::cerr << std::format("LOG [{}]: " fmt "\n", get_current_time() __VA_OPT__(,) __VA_ARGS__).c_str()))
#else
#define LOG(fmt, ...) ((void) 0)
#endif
