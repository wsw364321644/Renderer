#pragma once

/**
 *  @file GameFrameworkPCH.h
 *  @date September 30, 2020
 *  @author Jeremiah van Oosten
 *
 *  @brief Precompiled header file for the GameFramework.
 */



// STL
#include<filesystem>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <type_traits>
#include <io.h>
namespace fs = std::filesystem;

// Common lock type
using scoped_lock = std::lock_guard<std::mutex>;



// spdlog
#include <spdlog/async.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
