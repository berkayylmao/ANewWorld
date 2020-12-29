// clang-format off
//
//   The MIT License (MIT)
//   
//   Copyright (c) 2020 Berkay Yigit <berkaytgy@gmail.com>
//       Copyright holder detail: Nickname(s) used by the copyright holder: 'berkay2578', 'berkayylmao'.
//
//   Permission is hereby granted, free of charge, to any person obtaining a copy
//   of this software and associated documentation files (the "Software"), to deal
//   in the Software without restriction, including without limitation the rights
//   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//   copies of the Software, and to permit persons to whom the Software is
//   furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all
//   copies or substantial portions of the Software.
//
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//   SOFTWARE.
//
// clang-format on

#pragma once
#include <cstddef>  // size_t

namespace ImGui {
  class WithItemFlag {
    bool mIsFlagEnabled;

   public:
    WithItemFlag(ImGuiTabItemFlags option, bool enabled = true);
    ~WithItemFlag();

    operator bool();
  };
  class WithItemStyle {
    bool mIsStyleEnabled;

   public:
    WithItemStyle(ImGuiStyleVar style, float value, bool enabled = true);
    WithItemStyle(ImGuiStyleVar style, ImVec2 value, bool enabled = true);
    ~WithItemStyle();

    operator bool();
  };
  class WithItemWidth {
    bool mIsWidthEnabled;

   public:
    WithItemWidth(float width, bool enabled = true);
    ~WithItemWidth();

    operator bool();
  };

  IMGUI_API void LoadStyle();
  IMGUI_API bool FullWidthButton(const char* const str);
  IMGUI_API bool InputTextWithHintAlways(const char* label, const char* hint, char* buf, std::size_t buf_size,
                                         ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr,
                                         void* user_data = nullptr);
}  // namespace ImGui
