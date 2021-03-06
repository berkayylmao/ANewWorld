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
#include <cstddef>  // size_t

namespace ImGui {
  class WithItemColor {
    ImGuiCol mColorIdx;
    bool     mIsColorEnabled;
    ImVec4   mOldColor;

   public:
    explicit WithItemColor(ImGuiCol colorIdx, ImVec4 value, bool enabled = true);
    ~WithItemColor();

    explicit operator bool() const;
  };

  class WithItemFlag {
    bool mIsFlagEnabled;

   public:
    explicit WithItemFlag(ImGuiTabItemFlags option, bool enabled = true);
    ~WithItemFlag();

    explicit operator bool() const;
  };

  class WithItemStyle {
    bool mIsStyleEnabled;

   public:
    explicit WithItemStyle(ImGuiStyleVar style, float value, bool enabled = true);
    explicit WithItemStyle(ImGuiStyleVar style, ImVec2 value, bool enabled = true);
    ~WithItemStyle();

    explicit operator bool() const;
  };

  class WithItemWidth {
    bool mIsWidthEnabled;

   public:
    explicit WithItemWidth(float width, bool enabled = true);
    ~WithItemWidth();

    explicit operator bool() const;
  };

  IMGUI_API void LoadStyle();
  IMGUI_API auto FullWidthButton(const char* label) -> bool;
  IMGUI_API auto InputTextWithHintAlways(const char* label, const char* hint, char* buf, std::size_t buf_size,
                                         ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr,
                                         void* user_data = nullptr) -> bool;
  IMGUI_API void InformationMarker(const char* text);
}  // namespace ImGui
