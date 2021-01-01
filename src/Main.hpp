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
#include "pch.h"

#include "Extensions/D3D9.Base.hpp"

namespace ANewWorld {
  inline void Start() {
#if defined(_DEBUG)
    std::FILE* _hFreOpen = nullptr;
    AllocConsole();
    freopen_s(&_hFreOpen, "CONOUT$", "w", stdout);

    Logger::details::logLevel = LogLevel::Debug;
#endif

    // Handle logs directory + new log filename
    {
      std::stringstream _ss;
      auto              _time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      auto              _tm   = std::tm();
      localtime_s(&_tm, &_time);
      _ss << std::put_time(&_tm, "Log [%d-%m-%Y %H.%M.%S].log");

      const auto& _logFilesPath = Config::Get().GetConfigFolderPath() / "Logs";
      std::filesystem::create_directories(_logFilesPath);
      Logger::details::logFilePath = _logFilesPath / _ss.str();
    }

    Log(LogLevel::Info, fmt::format("Welcome to ANewWorld (v{}) by berkayylmao!", __MOD_VERSION_STR__));
    Log(LogLevel::Info, fmt::format("Parent commandline:\r\n\t{}", GetCommandLineA()));

    Log(LogLevel::Info, "Initializing extensions...");
    {
      Log(LogLevel::Info, "Setting up D3D9 extensions...");
      Extensions::D3D9::Init();
    }

    Log(LogLevel::Info, "Initialization complete!");
  }
} // namespace ANewWorld
