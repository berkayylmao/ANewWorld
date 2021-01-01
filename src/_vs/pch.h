// clang-format off
// 
//    ANewWorld (ANewWorld)
//    Copyright (C) 2020 Berkay Yigit <berkaytgy@gmail.com>
// 
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
// 
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
// 
//    You should have received a copy of the GNU Affero General Public License
//    along with this program. If not, see <https://www.gnu.org/licenses/>.
//
// clang-format on

#pragma once
#pragma warning(push, 0)  // disable any warnings from external headers

// Win32 targeting
#include <winsdkver.h>
#define _WIN32_WINNT 0x0501  // _WIN32_WINNT_WINXP
#include <sdkddkver.h>
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

// rapidJSON
#include <inc/rapidjson/document.h>
#include <inc/rapidjson/istreamwrapper.h>
#include <inc/rapidjson/ostreamwrapper.h>

// MirrorHook
#include <MirrorHook_ASI.hpp>

// OpenSpeed
#include <OpenSpeed/Core/MemoryEditor/MemoryEditor.hpp>

using namespace std::chrono_literals;

#pragma warning(pop)  // re-enable warnings

// Most used helpers
#include "Helpers/Config.hpp"
#include "Helpers/Logger.hpp"
#include "Helpers/Random.hpp"

// Mod info
#define __MOD_VERSION__ 1.0

// Helper macros
#define __STR__(x)          #x
#define __STR_EXPAND__(x)   __STR__(x)
#define __MOD_VERSION_STR__ __STR_EXPAND__(__MOD_VERSION__)
