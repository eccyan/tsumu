#pragma once

#include <string>

template <typename ... Args>
std::string format(const std::string& fmt, Args ... args);
