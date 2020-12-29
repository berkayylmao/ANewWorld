// clang-format off
//
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
#include <chrono>   // system_clock
#include <cstdint>  // integer types
#include <ctime>    // time_t
#include <fstream>  // ofstream
#include <iomanip>  // put_time
#include <sstream>
#include <string>
#include <stringapiset.h>  // MultiByteToWideChar

// fmt
#include "Helpers/fmt/format.h"
#include "Helpers/fmt/printf.h"
#pragma warning(pop)

enum class LogLevel : std::uint32_t { Debug, Info, Warning, Error };
namespace Logger {
  namespace details {
    static LogLevel              logLevel = LogLevel::Info;
    static std::filesystem::path logFilePath;
  }  // namespace details

  static void Log(LogLevel level, const wchar_t* message, const wchar_t* const functionName,
                  const wchar_t* const fileName, const std::int32_t lineNumber) {
    if (level < details::logLevel) return;

    std::wstringstream _ss;
    {
      std::time_t _time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::tm     _tm;
      localtime_s(&_tm, &_time);
      _ss << std::put_time(&_tm, L"%d/%m/%Y %H:%M:%S");
    }

    static std::wstring _header, _content;
    switch (level) {
      case LogLevel::Debug:
        _header = L"DEBUG";
        break;
      case LogLevel::Info:
        _header = L"INFO";
        break;
      case LogLevel::Warning:
        _header = L"WARNING";
        break;
      case LogLevel::Error:
        _header = L"ERROR";
        break;
    }
    _content = fmt::sprintf(L"[%ls] %ls - %ls\r\n\tFrom: %ls (%ls:%d)\r\n", _header, _ss.str(), message, functionName,
                            fileName, lineNumber);

    if (details::logLevel == LogLevel::Debug) std::wprintf(L"%ls", _content.c_str());

    std::wofstream _ofs(details::logFilePath, std::ios_base::binary | std::ios_base::app);
    _ofs << _content << std::endl;
  }
  static inline void Log(LogLevel level, const std::wstring& message, const wchar_t* const functionName,
                         const wchar_t* const fileName, const std::int32_t lineNumber) {
    Log(level, message.c_str(), functionName, fileName, lineNumber);
  }
  static inline void Log(LogLevel level, const char* message, const wchar_t* const functionName,
                         const wchar_t* const fileName, const std::int32_t lineNumber) {
    auto _wsize = MultiByteToWideChar(CP_UTF8, 0, message, std::strlen(message), NULL, 0);
    if (auto _wszMsg = new wchar_t[_wsize + 1]()) {
      MultiByteToWideChar(CP_UTF8, 0, message, std::strlen(message), _wszMsg, _wsize);
      Log(level, _wszMsg, functionName, fileName, lineNumber);
      delete[] _wszMsg;
    }
  }
  static inline void Log(LogLevel level, const std::string& message, const wchar_t* const functionName,
                         const wchar_t* const fileName, const std::int32_t lineNumber) {
    Log(level, message.c_str(), functionName, fileName, lineNumber);
  }
}  // namespace Logger

#define __Log__ExpandLayer(str) L##str
#define __Log__Expand(str)      __Log__ExpandLayer(str)
#define Log(LogLevel_, Message) \
  Logger::Log(LogLevel_, Message, __Log__Expand(__FUNCSIG__), __Log__Expand(__FILE__), __LINE__);\
