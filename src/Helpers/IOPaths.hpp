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
#pragma warning(push, 0)
#include <filesystem>
#include <libloaderapi.h> // GetModuleFileName
#pragma warning(pop)

namespace IOPaths {
  namespace details {
    // Filenames

    constexpr static char CONST_configJson[] = "Config.json";

    // Directories

    constexpr static char CONST_assetsDir[] = "ANewWorld";
    constexpr static char CONST_dataDir[]   = ".anw_data";
    constexpr static char CONST_logsDir[]   = "Logs";

    inline static auto getRootDir() -> const auto& {
      static std::filesystem::path _rootDir;
      if (_rootDir.empty()) {
        std::wstring _strPath(2048, 0);
        ::GetModuleFileNameW(nullptr, &_strPath[0], _strPath.capacity());

        _rootDir.assign(_strPath);
        _rootDir = _rootDir.parent_path();
      }

      return _rootDir;
    }
  } // namespace details

  // Files
  namespace Files {
    [[nodiscard]] inline static auto GetConfigJsonPath() -> auto {
      return details::getRootDir() / details::CONST_dataDir / details::CONST_configJson;
    }
  } // namespace Files

  // Directories
  namespace Directories {
    [[nodiscard]] inline static auto GetAssetsDir() -> auto {
      auto _dir = details::getRootDir() / details::CONST_assetsDir;
      std::filesystem::create_directories(_dir);

      return _dir;
    }

    [[nodiscard]] inline static auto GetDataDir() -> auto {
      auto _dir = details::getRootDir() / details::CONST_dataDir;
      std::filesystem::create_directories(_dir);

      return _dir;
    }

    [[nodiscard]] inline static auto GetLogsDir() -> auto {
      auto _dir = details::getRootDir() / details::CONST_dataDir / details::CONST_logsDir;
      std::filesystem::create_directories(_dir);

      return _dir;
    }
  } // namespace Directories
}   // namespace IOPaths
