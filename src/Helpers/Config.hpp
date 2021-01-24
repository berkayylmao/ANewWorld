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
#include <WinUser.h> // MessageBox

// rapidJSON
#include <inc/rapidjson/document.h>
#include <inc/rapidjson/prettywriter.h>
#include <inc/rapidjson/istreamwrapper.h>
#include <inc/rapidjson/ostreamwrapper.h>
#pragma warning(pop)

// Paths
#include "Helpers/IOPaths.hpp"

class Config {
  static constexpr char CONST_defConfigJson[] =
    R"({"ConfigFileVersion":"1.0","IsFirstTimeUser":true,"NewLoadingScreens":{"Enabled":true,"BackgroundTimer":5.0,"LoadingTextResolution":"Custom"}})";

  rapidjson::Document mConfigDocument;

  public:
  void Save() const {
    try {
      std::filesystem::create_directories(IOPaths::Directories::GetDataDir());
      {
        std::ofstream                                      _ofs(IOPaths::Files::GetConfigJsonPath(), std::ios_base::binary);
        rapidjson::OStreamWrapper                          _osw(_ofs);
        rapidjson::PrettyWriter<rapidjson::OStreamWrapper> _writer(_osw);
        _writer.SetIndent(' ', 3);
        mConfigDocument.Accept(_writer);
      }
    }
    catch (const std::runtime_error& e) { ::MessageBoxA(nullptr, e.what(), "Error saving config!", MB_ICONERROR | MB_OK); }
  }

  auto operator[](const char* const szConfigName) -> rapidjson::Value& { return mConfigDocument[szConfigName]; }

  private:
  void Load() {
    try {
      if (!std::filesystem::exists(IOPaths::Files::GetConfigJsonPath())) {
        mConfigDocument.Parse(CONST_defConfigJson);
        Save();
      }
      else {
        std::ifstream             _ifs(IOPaths::Files::GetConfigJsonPath(), std::ios_base::binary);
        rapidjson::IStreamWrapper _isw(_ifs);
        mConfigDocument.ParseStream(_isw);

        // TODO: check for 'ConfigFileVersion'
      }
    }
    catch (const std::runtime_error& e) { ::MessageBoxA(nullptr, e.what(), "Error loading config!", MB_ICONERROR | MB_OK); }
  }

  explicit Config() { Load(); }

  public:
  static auto Get() -> Config& {
    static Config _instance{};

    return _instance;
  }
};
