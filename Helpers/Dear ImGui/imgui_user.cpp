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

#include <cstdint>  // integer types

#include "imgui.h"
#include "imgui_internal.h"

namespace ImGui {
#pragma region WithItemFlag
  WithItemFlag::WithItemFlag(ImGuiTabItemFlags option, bool enabled) : mIsFlagEnabled(enabled) {
    ImGui::PushItemFlag(option, enabled);
  }
  WithItemFlag::~WithItemFlag() { ImGui::PopItemFlag(); }
  WithItemFlag::operator bool() { return mIsFlagEnabled; }
#pragma endregion
#pragma region WithItemStyle
  WithItemStyle::WithItemStyle(ImGuiStyleVar style, float value, bool enabled) : mIsStyleEnabled(enabled) {
    if (enabled) ImGui::PushStyleVar(style, value);
  }
  WithItemStyle::WithItemStyle(ImGuiStyleVar style, ImVec2 value, bool enabled) : mIsStyleEnabled(enabled) {
    if (enabled) ImGui::PushStyleVar(style, value);
  }
  WithItemStyle::~WithItemStyle() {
    if (mIsStyleEnabled) ImGui::PopStyleVar();
  }
  WithItemStyle::operator bool() { return mIsStyleEnabled; }
#pragma endregion
#pragma region WithItemWidth
  WithItemWidth::WithItemWidth(float width, bool enabled) : mIsWidthEnabled(enabled) {
    if (enabled) ImGui::PushItemWidth(width);
  }
  WithItemWidth::~WithItemWidth() {
    if (mIsWidthEnabled) ImGui::PopItemWidth();
  }
  WithItemWidth::operator bool() { return mIsWidthEnabled; }
#pragma endregion

  /// Misc.

  IMGUI_API void LoadStyle() {
    auto& style                    = ImGui::GetStyle();
    style.Alpha                    = 0.85f;
    style.WindowPadding            = ImVec2(10.0f, 10.0f);
    style.WindowRounding           = 1.0f;
    style.WindowBorderSize         = 0.0f;
    style.WindowMinSize            = ImVec2(450.0f, 200.0f);
    style.WindowTitleAlign         = ImVec2(0.0f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.ChildRounding            = 1.0f;
    style.ChildBorderSize          = 0.0f;
    style.PopupRounding            = 2.0f;
    style.PopupBorderSize          = 1.0f;
    style.FramePadding             = ImVec2(5.0f, 2.0f);
    style.FrameRounding            = 1.0f;
    style.FrameBorderSize          = 0.0f;
    style.TabRounding              = 1.0f;
    style.TabBorderSize            = 0.0f;
    style.ItemSpacing              = ImVec2(5.0f, 5.0f);
    style.ItemInnerSpacing         = ImVec2(5.0f, 5.0f);
    style.IndentSpacing            = 25.0f;
    style.ScrollbarSize            = 10.0f;
    style.ScrollbarRounding        = 1.0f;
    style.GrabMinSize              = 5.0f;
    style.GrabRounding             = 1.0f;
    style.ButtonTextAlign          = ImVec2(0.5f, 0.5f);
    style.AntiAliasedLines         = true;
    style.AntiAliasedFill          = true;

    style.Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.70f, 0.44f, 0.00f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.80f, 0.50f, 0.00f, 0.40f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.80f, 0.40f, 0.03f, 0.80f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.89f, 0.49f, 0.03f, 0.80f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.30f, 0.30f, 0.30f, 0.80f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.50f, 0.50f, 0.50f, 0.80f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.89f, 0.49f, 0.03f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.80f, 0.44f, 0.02f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.89f, 0.49f, 0.03f, 1.00f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.80f, 0.44f, 0.02f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(1.00f, 0.56f, 0.04f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.89f, 0.49f, 0.03f, 1.00f);
    style.Colors[ImGuiCol_Header]                = ImVec4(1.00f, 0.63f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(1.00f, 0.63f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(1.00f, 0.63f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_Separator]             = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.80f, 0.50f, 0.00f, 0.40f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.90f, 0.60f, 0.00f, 0.80f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(1.00f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_Tab]                   = ImVec4(0.89f, 0.49f, 0.03f, 0.50f);
    style.Colors[ImGuiCol_TabHovered]            = ImVec4(1.00f, 0.56f, 0.04f, 1.00f);
    style.Colors[ImGuiCol_TabActive]             = ImVec4(0.89f, 0.49f, 0.03f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.89f, 0.49f, 0.03f, 1.00f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.85f, 0.53f, 0.00f, 0.40f);
    style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
  }
  IMGUI_API bool FullWidthButton(const char* const label) {
    const ImVec2 _regionAvail = ImGui::GetContentRegionAvail();
    const ImVec2 _sizeLabel   = ImGui::CalcTextSize(label);
    const ImVec2 _size =
        _sizeLabel.x > _regionAvail.x ? _sizeLabel + (ImGui::GetStyle().FramePadding * 2.0f) : _regionAvail;
    return ImGui::Button(label, ImVec2(_size.x, 0.0f));
  }
  IMGUI_API bool InputTextWithHintAlways(const char* label, const char* hint, char* buf, std::size_t buf_size,
                                         ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data) {
    ImGuiWindow* _window = GetCurrentWindow();
    if (_window->SkipItems) return false;

    const ImGuiID       _id        = _window->GetID(label);
    const ImGuiContext& _ctx       = *GImGui;
    const ImVec2        _cursorPos = ImGui::GetCursorPos();
    const bool          _isActive  = _ctx.InputTextState.ID == _id;

    const bool _isTextChanged = ImGui::InputTextEx(label, nullptr, buf, static_cast<std::int32_t>(buf_size),
                                                   ImVec2(0, 0), flags, callback, user_data);

    ImGui::SetCursorPosX(_cursorPos.x + ImGui::GetItemRectSize().x - ImGui::CalcTextSize(hint).x -
                         ImGui::GetStyle().FramePadding.x);
    ImGui::SetCursorPosY(_cursorPos.y + ImGui::GetStyle().FramePadding.y);
    ImGui::TextDisabled(hint);

    return _isTextChanged;
  }
}  // namespace ImGui
