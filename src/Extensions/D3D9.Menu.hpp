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

#pragma warning(push, 0)
#include <shellapi.h>  // ShellExecute

// Dear ImGui
#include "Helpers/Dear ImGui/imgui.h"
#pragma warning(pop)

namespace Extensions::D3D9::Menu {
  namespace details {
#pragma region TABS
#pragma region Config(TAB)
    /// CONFIG TAB

    // Constants

    static constexpr char CONFIG_szNewLoadingScreensHeader[]                 = "New Loading Screens (BETA)";
    static constexpr char CONFIG_szNewLoadingScreensEnabled[]                = "Enabled";
    static constexpr char CONFIG_szNewLoadingScreensNextBackgroundDuration[] = "Background duration (seconds)";
    static constexpr char CONFIG_szNewLoadingScreensLoadingTextResolution[]  = "Loading text resolution";

    // Tab Content
    static void DrawNewLoadingScreensConfigTabContent() {
      if (ImGui::BeginChild("###NewLoadingScreensConfigChild", {0.0f, 0.0f}, false, ImGuiWindowFlags_NoScrollbar)) {
        static auto& _newLoadingScreensConfig = Config::Get()["NewLoadingScreens"];

        // Enabled checkbox
        {
          ImGui::AlignTextToFramePadding();
          ImGui::TextUnformatted(CONFIG_szNewLoadingScreensEnabled);
          ImGui::SameLine();
          ImGui::InformationMarker("Whether ANewWorld should show new loading screens.");

          // Special bool handling
          static auto _enabled = _newLoadingScreensConfig["Enabled"].GetBool();

          ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImGui::GetStyle().WindowPadding.x * 2.0f);
          if (ImGui::Checkbox("###NewLoadingScreensEnabledCheckbox", &_enabled)) {
            _newLoadingScreensConfig["Enabled"].SetBool(_enabled);
            Config::Get().Save();
          }
        }

        // Background timer
        {
          ImGui::AlignTextToFramePadding();
          ImGui::TextUnformatted(CONFIG_szNewLoadingScreensNextBackgroundDuration);
          ImGui::SameLine();
          ImGui::InformationMarker("How often ANewWorld will switch to a different background.");

          ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 100.0f);
          ImGui::WithItemWidth _w(100.0f);
          ImGui::InputScalar("###NewLoadingScreensBackgroundTimer", ImGuiDataType_Float,
                             reinterpret_cast<float*>(&_newLoadingScreensConfig["BackgroundTimer"]), nullptr, nullptr,
                             "%.2f");
          if (ImGui::IsItemDeactivatedAfterEdit()) Config::Get().Save();
        }

        // Loading text resolution
        {
          // Dirty, but quick
          static auto _resolutionIdx = 0;
          {
            const auto _sz = Config::Get()["NewLoadingScreens"]["LoadingTextResolution"].Get<std::string>();
            if (_sz._Equal("360p"))
              _resolutionIdx = 0;
            else if (_sz._Equal("720p"))
              _resolutionIdx = 1;
            else
              _resolutionIdx = 2;
          }

          ImGui::AlignTextToFramePadding();
          ImGui::TextUnformatted(CONFIG_szNewLoadingScreensLoadingTextResolution);
          ImGui::SameLine();
          ImGui::InformationMarker("The resolution will be changed in the next launch.");

          ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 100.0f);
          ImGui::WithItemWidth _w(100.0f);
          if (ImGui::Combo("###NewLoadingScreensLoadingTextResolution", &_resolutionIdx, " 360p\0 720p\0 Custom\0")) {
            static std::string _sz;
            switch (_resolutionIdx) {
              default:
              case 0:
                _sz = "360p";
                break;
              case 1:
                _sz = "720p";
                break;
              case 2:
                _sz = "Custom";
                break;
            }
            Config::Get()["NewLoadingScreens"]["LoadingTextResolution"].SetString(_sz.c_str(), _sz.length());
            Config::Get().Save();
          }
        }
      }

      ImGui::EndChild();
    }
#pragma endregion
#pragma region About(TAB)
    /// ABOUT TAB

    // Constants

    static constexpr char ABOUT_szLinkGitHub[]        = "https://github.com/berkayylmao/ANewWorld/";
    static constexpr char ABOUT_szLinkGitHubRelease[] = "https://github.com/berkayylmao/ANewWorld/releases/latest";
    static constexpr char ABOUT_szLinkPayPal[]        = "https://www.paypal.me/berkayylmao";
    static constexpr char ABOUT_szLinkPaysafecard[]   = "https://localhost";
    static constexpr char ABOUT_szLinkPatreon[]       = "https://localhost";

    // Tab content
    static void DrawAboutTabContent() {
      ImGui::TextUnformatted("ANewWorld v" __MOD_VERSION_STR__ "\nCopyright (C) berkayylmao. All rights reserved.");

      // GitHub
      {
        ImGui::TextUnformatted("Stay up to date with the latest version from ");
        ImGui::SameLine(0.0f, 0.0f);
        if (ImGui::SmallButton("GitHub"))
          ShellExecuteA(nullptr, nullptr, details::ABOUT_szLinkGitHubRelease, nullptr, nullptr, SW_SHOW);
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::TextUnformatted(".");
      }

      // Donation
      {
        ImGui::Separator();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 30.0f);
        ImGui::TextUnformatted(
            "Making and maintaining a mod like this requires a lot of\n"
            "time and energy. If you enjoyed this mod, please consider donating!");
        ImGui::PopTextWrapPos();
        if (ImGui::SmallButton("PayPal"))
          ShellExecuteA(nullptr, nullptr, details::ABOUT_szLinkPayPal, nullptr, nullptr, SW_SHOW);
        // ImGui::SameLine();
        // if (ImGui::SmallButton("Paysafecard"))
        //   ShellExecuteA(nullptr, nullptr, details::ABOUT_szLinkPaysafecard, nullptr, nullptr, SW_SHOW);
        // ImGui::SameLine();
        // if (ImGui::SmallButton("Patreon")) ShellExecuteA(nullptr, nullptr, details::ABOUT_szLinkPatreon, nullptr,
        //                                                  nullptr, SW_SHOW);
      }

      // Compatibility
      {
        ImGui::Separator();
        ImGui::TextUnformatted("Compatibility:");
        ImGui::Indent();
        {
          ImGui::BulletText("This mod is compatible with NFS: World (build 1613).");
          ImGui::BulletText("Besides offline play, only WU.GG servers are officially supported.");
        }
        ImGui::Unindent();
      }
    }
#pragma endregion
#pragma endregion
  } // namespace details

  // Called when user has Configuration tab active
  static void DrawConfig() {
    if (ImGui::BeginTabBar("###ConfigTabBar", ImGuiTabBarFlags_FittingPolicyResizeDown)) {
      if (ImGui::BeginTabItem(details::CONFIG_szNewLoadingScreensHeader)) {
        details::DrawNewLoadingScreensConfigTabContent();
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
  }

  // Called when user has About tab active
  static void DrawAbout() { details::DrawAboutTabContent(); }
} // namespace Extensions::D3D9::Menu
