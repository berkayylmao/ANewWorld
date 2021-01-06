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

// Menu
#include "Extensions/D3D9.Menu.hpp"

// Features
#include "Extensions/NewLoadingScreens/NewLoadingScreens.hpp"

#pragma warning(push, 0)
// Dear ImGui
#include "Helpers/Dear ImGui/imgui.h"
#include "Helpers/Dear ImGui/imgui_internal.h"
#include "Helpers/Dear ImGui/Fonts/OpenSans-SemiBold.hpp"
#include "Helpers/Dear ImGui/Implementations/imgui_impl_dx9.h"
#include "Helpers/Dear ImGui/Implementations/imgui_impl_win32.h"
extern IMGUI_IMPL_API auto ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
#pragma warning(pop)

namespace Extensions::D3D9 {
  namespace details {
    /// CONSTANTS

    constexpr static char CONSTANT_szUITitleMain[]   = "ANewWorld";
    constexpr static char CONSTANT_szUITitleConfig[] = "Configuration";
    constexpr static char CONSTANT_szUITitleAbout[]  = "About";

    /// FIELDS

    static bool isMainMenuVisible = true;

    /// HOOKS

    static void __stdcall BeginScene(IDirect3DDevice9*) {
      ImGui_ImplDX9_NewFrame();
      ImGui_ImplWin32_NewFrame();
      ImGui::NewFrame([]() { ImGui::Begin("Debug###Main", &isMainMenuVisible, ImGuiWindowFlags_AlwaysAutoResize); });

      ImGui::GetIO().FontGlobalScale = std::max(0.8f, ImGui::GetIO().DisplaySize.y / 1080.0f); // 1080p as base
    }

    static void __stdcall EndScene(IDirect3DDevice9* pDevice) {
      if (!ImGui::GetCurrentContext()->WithinFrameScope) return;

      // Features
      NewLoadingScreens::Draw(pDevice);

      if (isMainMenuVisible) {
        // if (ImGui::Begin(CONSTANT_szUITitleMain, &isMainMenuVisible,
        //                 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)) {
        //  isMainMenuVisible = !ImGui::FullWidthButton("Minimize");
        //}
        // ImGui::End();

        if (ImGui::Begin(CONSTANT_szUITitleConfig, &isMainMenuVisible,
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
          Menu::DrawConfig();
        ImGui::End();

        if (ImGui::Begin(CONSTANT_szUITitleAbout, &isMainMenuVisible,
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
          Menu::DrawAbout();
        ImGui::End();

        // Dock previous UIs together
        {
          ImGui::SetNextWindowPos({10.0f, 10.f}, ImGuiCond_Always);
          const auto _dockID = ImGui::GetID("MainWindowDockingUI");
          if (!ImGui::DockBuilderGetNode(_dockID)) {
            ImGui::DockBuilderRemoveNode(_dockID);
            ImGui::DockBuilderAddNode(
                _dockID,
                ImGuiDockNodeFlags_NoSplit | ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_NoWindowMenuButton,
                &isMainMenuVisible);

            // ImGui::DockBuilderDockWindow(CONSTANT_szUITitleMain, _dockID);
            ImGui::DockBuilderDockWindow(CONSTANT_szUITitleConfig, _dockID);
            ImGui::DockBuilderDockWindow(CONSTANT_szUITitleAbout, _dockID);

            ImGui::DockBuilderFinish(_dockID);
          }
        }
      } else {
        const auto  _textSize    = ImGui::CalcTextSize(CONSTANT_szUITitleMain) * 0.8f;
        const auto& _displaySize = ImGui::GetIO().DisplaySize;

        ImGui::WithItemStyle _style1(ImGuiStyleVar_FramePadding, {5.0f, 0.0f});
        ImGui::WithItemStyle _style2(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::SetNextWindowContentSize({_textSize.y, _textSize.x});
        ImGui::SetNextWindowPos({_displaySize.x - _textSize.x - 210.0f, _displaySize.y - _textSize.y},
                                ImGuiCond_Always);
        if (ImGui::Begin("###MainMenuToggle", nullptr,
                         ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_AlwaysAutoResize)) {
          ImGui::SetWindowFontScale(0.8f);

          ImGui::WithItemColor _color1(ImGuiCol_Button,
                                       ImGui::GetStyleColorVec4(ImGuiCol_Button) - ImVec4(0.0f, 0.0f, 0.0f, 0.7f));
          ImGui::WithItemColor _color2(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered) -
                                                               ImVec4(0.0f, 0.0f, 0.0f, 0.7f));
          ImGui::WithItemColor _color3(
              ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive) - ImVec4(0.0f, 0.0f, 0.0f, 0.7f));
          ImGui::WithItemColor _color4(ImGuiCol_Text,
                                       ImGui::GetStyleColorVec4(ImGuiCol_Text) - ImVec4(0.0f, 0.0f, 0.0f, 0.7f));
          isMainMenuVisible = ImGui::Button(CONSTANT_szUITitleMain);
        }
        ImGui::End();
      }

      ImGui::GetIO().MouseDrawCursor = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
      ImGui::Render();
      ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }

    static void __stdcall BeforeReset(IDirect3DDevice9*, void*) { ImGui_ImplDX9_InvalidateDeviceObjects(); }
    static void __stdcall AfterReset(IDirect3DDevice9*, void*) { ImGui_ImplDX9_CreateDeviceObjects(); }

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
      if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) return TRUE;
      return MirrorHook::WndProc::g_constIgnoreThisReturn;
    }
  } // namespace details

  static void Init() {
    IDirect3DDevice9* _pD3D9Dev = nullptr;
    Log(LogLevel::Debug, "Waiting for D3D9 device...");
    {
      auto** _ppD3DDev = reinterpret_cast<IDirect3DDevice9**>(MemoryEditor::Get().AbsRVA(0x9064A8));
      while (_pD3D9Dev == nullptr) {
        _pD3D9Dev = *_ppD3DDev;
        std::this_thread::sleep_for(1s);
      }
    }

    Log(LogLevel::Info, "Setting up MirrorHook...");
    while (!MirrorHook::Init(MirrorHook::Framework::D3D9, &_pD3D9Dev)) std::this_thread::sleep_for(1s);

    Log(LogLevel::Info, "Setting up Dear ImGui...");
    {
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();

      auto& _io = ImGui::GetIO();
      _io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
      _io.Fonts->AddFontFromMemoryCompressedTTF(ImGui::Fonts::OpenSans::SemiBold_compressed_data,
                                                ImGui::Fonts::OpenSans::SemiBold_compressed_size, 24.0f);
      _io.FontDefault = nullptr;
      _io.IniFilename = nullptr;
      ImGui::LoadStyle();

      ImGui_ImplWin32_Init(MirrorHook::WndProc::GetWindowHandle());
      ImGui_ImplDX9_Init(_pD3D9Dev);
    }

    Log(LogLevel::Info, "Adding MirrorHook extensions...");
    {
      MirrorHook::D3D9::AddExtension(MirrorHook::D3D9::Type::BeginScene, &details::BeginScene);
      MirrorHook::D3D9::AddExtension(MirrorHook::D3D9::Type::EndScene, &details::EndScene);
      MirrorHook::D3D9::AddExtension(MirrorHook::D3D9::Type::BeforeReset, &details::BeforeReset);
      MirrorHook::D3D9::AddExtension(MirrorHook::D3D9::Type::AfterReset, &details::AfterReset);

      MirrorHook::WndProc::AddExtension(&details::WndProc);
    }

    Log(LogLevel::Info, "Initializing features...");
    {
      NewLoadingScreens::Init();
    }
  }
} // namespace Extensions::D3D9
