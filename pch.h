#pragma once
#pragma warning(push, 0)  // disable any warnings from external headers

// Win32 targeting
#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0601  // _WIN32_WINNT_WIN7
#include <SDKDDKVer.h>
// Win32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
// Commonly used headers
#include <algorithm>  // find(), max_element()
#include <chrono>
#include <cstddef>  // size_t
#include <cstdint>  // integer types
#include <filesystem>
#include <list>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

// fmt
#include "Helpers/fmt/format.h"
#include "Helpers/fmt/printf.h"

// MirrorHook
#include <MirrorHook_ASI.hpp>

// OpenSpeed
#include <OpenSpeed/Core/MemoryEditor/MemoryEditor.hpp>

using namespace std::chrono_literals;

#pragma warning(pop)  // re-enable warnings

#include "Helpers/Config.hpp"
#include "Helpers/Logger.hpp"
#include "Helpers/Random.hpp"
#include "Helpers/TwitchIRC.hpp"

// Mod info
#define __STR__(x)          #x
#define __STR_EXPAND__(x)   __STR__(x)
#define __MOD_VERSION__     1.0
#define __MOD_VERSION_STR__ __STR_EXPAND__(__MOD_VERSION__)
