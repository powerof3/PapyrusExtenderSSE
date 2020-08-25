#pragma once

#include "RE/Skyrim.h"
#include "REL/Relocation.h"
#include "SKSE/SKSE.h"

#include <frozen/map.h>
#include <frozen/set.h>
#include <frozen/string.h>
#include <spdlog/sinks/basic_file_sink.h>

#ifndef NDEBUG
#include <spdlog/sinks/msvc_sink.h>
#endif

namespace logger = SKSE::log;
using namespace SKSE::util;

#define DLLEXPORT __declspec(dllexport)
