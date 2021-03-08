#pragma once

#include "RE/Skyrim.h"
#include "xbyak/xbyak.h"
#include "SKSE/SKSE.h"

#include <frozen/map.h>
#include <frozen/unordered_map.h>
#include <frozen/set.h>
#include <frozen/string.h>
#include <spdlog/sinks/basic_file_sink.h>

#ifndef NDEBUG
#include <spdlog/sinks/msvc_sink.h>
#endif

namespace logger = SKSE::log;
namespace stl = SKSE::stl;

using namespace SKSE::util;
using namespace std::string_view_literals;

#define DLLEXPORT __declspec(dllexport)
