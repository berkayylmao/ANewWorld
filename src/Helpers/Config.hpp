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
#include <fstream>
#include <filesystem>
#include <libloaderapi.h>  // GetModuleFileName
#include <WinUser.h>       // MessageBox

// rapidJSON
#include <inc/rapidjson/document.h>
#include <inc/rapidjson/prettywriter.h>
#include <inc/rapidjson/istreamwrapper.h>
#include <inc/rapidjson/ostreamwrapper.h>
#pragma warning(pop)

class Config {
  // File paths

  constexpr static char CONSTANT_pathConfigFileName[] = "Config.json";
  constexpr static char CONSTANT_pathConfigFolder[]   = "ANewWorld";

protected:
  std::filesystem::path mConfigFolderPath;
  std::filesystem::path mConfigFilePath;

  rapidjson::Document mConfigDocument;

public:
  [[nodiscard]] auto GetConfigFolderPath() const -> const auto& { return mConfigFolderPath; }

  void Save() const {
    try {
      std::filesystem::create_directories(mConfigFolderPath);
      {
        std::ofstream                                      _ofs(mConfigFilePath, std::ios_base::binary);
        rapidjson::OStreamWrapper                          _osw(_ofs);
        rapidjson::PrettyWriter<rapidjson::OStreamWrapper> _writer(_osw);
        _writer.SetIndent(' ', 3);
        mConfigDocument.Accept(_writer);
      }
    } catch (const std::runtime_error& e) {
      ::MessageBoxA(nullptr, e.what(), "Error saving config!", MB_ICONERROR | MB_OK);
    }
  }

  auto operator[](const char* const szConfigName) -> rapidjson::Value& { return mConfigDocument[szConfigName]; }

private:
  void Load() {
    try {
      if (!std::filesystem::exists(mConfigFilePath)) {
        ::MessageBoxA(nullptr, "Config file is missing", "Error loading config!", MB_ICONERROR | MB_OK);
      } else {
        std::ifstream             _ifs(mConfigFilePath, std::ios_base::binary);
        rapidjson::IStreamWrapper _isw(_ifs);
        mConfigDocument.ParseStream(_isw);
      }
    } catch (const std::runtime_error& e) {
      ::MessageBoxA(nullptr, e.what(), "Error loading config!", MB_ICONERROR | MB_OK);
    }
  }

  explicit Config() {
    std::wstring _strPath(2048, 0);
    ::GetModuleFileNameW(nullptr, &_strPath[0], _strPath.capacity());
    const std::filesystem::path _path(_strPath.c_str());

    mConfigFolderPath = _path.parent_path() / CONSTANT_pathConfigFolder;
    mConfigFilePath   = mConfigFolderPath / CONSTANT_pathConfigFileName;
    Load();
  }

public:
  static auto Get() -> Config& {
    static Config _instance;
    return _instance;
  }
};
