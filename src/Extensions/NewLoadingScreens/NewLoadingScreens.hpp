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
// D3D9
#include <d3d9.h>
#include <d3dx9.h>

// Dear ImGui
#include "Helpers/Dear ImGui/imgui.h"
#include "Helpers/Dear ImGui/imgui_internal.h"  // math operators for ImVec2
#pragma warning(pop)

namespace Extensions::D3D9::NewLoadingScreens {
  namespace details {
    namespace Hooks {
      static inline bool isShowingLoadingScreen = false;
      static inline bool resetTimers            = false;

      static void __declspec(naked) hkLoadingScreen_LoadScreen() {
        __asm {
          // mov[Extensions::InGameMenu::hasUserSeenFirstLoadingScreen], 1
            mov[isShowingLoadingScreen], 1
            mov edx, [ecx]
            mov edx, [edx+0x5C]
            ret
        }
      }

      static void __declspec(naked) hkLoadingScreen_UnloadScreen() {
        __asm {
            mov[isShowingLoadingScreen], 0
            mov[resetTimers], 1
            pop edi
            pop esi
            mov esp, ebp
            pop ebp
            retn 0x10
        }
      }
    }  // namespace Hooks

    struct LoadingTextSprite {
      std::filesystem::path           mPath;
      std::vector<LPDIRECT3DTEXTURE9> mSprites;
      std::uint32_t                   mFrames{0};
      std::uint32_t                   mWidth{0};
      std::uint32_t                   mHeight{0};
      float                           mRenderSizeFactor{4.0f};

      explicit LoadingTextSprite() {
        this->mPath =
            Config::Get().GetConfigFolderPath() / "NewLoadingScreens" /
            fmt::format("LoadingText_{}", Config::Get()["NewLoadingScreens"]["LoadingTextResolution"].GetString());
        const auto _spriteJson = this->mPath / "sprite.json";

        try {
          if (!std::filesystem::exists(_spriteJson)) {
            ::MessageBoxA(nullptr, "Sprite files are missing (for 'LoadingText')!", "Error loading 'LoadingText'!",
                          MB_ICONERROR | MB_OK);
          } else {
            rapidjson::Document _doc;

            std::ifstream             _ifs(_spriteJson, std::ios_base::binary);
            rapidjson::IStreamWrapper _isw(_ifs);
            _doc.ParseStream(_isw);

            this->mFrames           = _doc["Frames"].GetUint();
            this->mWidth            = _doc["Width"].GetUint();
            this->mHeight           = _doc["Height"].GetUint();
            this->mRenderSizeFactor = _doc["RenderSizeFactor"].GetFloat();

            this->mSprites.resize(this->mFrames);
          }
        } catch (const std::runtime_error& e) {
          ::MessageBoxA(nullptr, e.what(), "Error loading sprite (for 'LoadingText')!", MB_ICONERROR | MB_OK);
        }
      }

      ~LoadingTextSprite() = default;
    };

    static inline bool alreadyCreatingTextures = false;
    static inline bool texturesCreated         = false;

    static inline LoadingTextSprite               loadingTextSprite;
    static inline std::vector<LPDIRECT3DTEXTURE9> vTextures;

    static void CreateTextures(LPDIRECT3DDEVICE9 pDevice) {
      Log(LogLevel::Info, "Creating textures...");
      alreadyCreatingTextures = true;

      HRESULT _result;
      {
        const auto _dir = Config::Get().GetConfigFolderPath() / "NewLoadingScreens" / "Backgrounds";
        Log(LogLevel::Debug, fmt::format("Textures folder: {}", _dir.string()));

        for (const auto& element : std::filesystem::directory_iterator(_dir)) {
          Log(LogLevel::Debug, fmt::format("Processing '{}'", element.path().u8string()));

          LPDIRECT3DTEXTURE9 _dummyTexture;
          if (FAILED(_result = D3DXCreateTextureFromFileExW(
                         pDevice, element.path().wstring().c_str(), loadingTextSprite.mWidth, loadingTextSprite.mHeight,
                         D3DX_FROM_FILE, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE, D3DX_DEFAULT, 0,
                         nullptr, nullptr, &_dummyTexture)))
            Log(LogLevel::Error, fmt::format("Texture couldn't be created.\n\tResult:\n\tFile:{}\n\t",
                                             element.path().u8string(), _result));
          else
            vTextures.push_back(_dummyTexture);
        }
      }

      Log(LogLevel::Info, "Creating LoadingText texture...");
      {
        Log(LogLevel::Debug, fmt::format("Textures folder: {}", loadingTextSprite.mPath.u8string()));

        for (std::size_t i = 0; i < loadingTextSprite.mFrames; i++) {
          const auto _file = (loadingTextSprite.mPath / (std::to_string(i) + ".png"));

          if (FAILED(_result = D3DXCreateTextureFromFileExW(
                         pDevice, _file.wstring().c_str(), loadingTextSprite.mWidth, loadingTextSprite.mHeight,
                         D3DX_FROM_FILE, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE, D3DX_DEFAULT, 0,
                         nullptr, nullptr, &loadingTextSprite.mSprites[i])))
            Log(LogLevel::Error,
                fmt::format("Texture couldn't be created.\n\tResult:\n\tFile:{}\n\t", _file.u8string(), _result));
        }
      }

      texturesCreated         = true;
      alreadyCreatingTextures = false;
    }

  }  // namespace details

  static void Draw(IDirect3DDevice9* pDevice) {
    if (!Config::Get()["NewLoadingScreens"]["Enabled"].GetBool()) return;
    if (!details::texturesCreated) {
      if (!details::alreadyCreatingTextures) std::thread(details::CreateTextures, pDevice).detach();
      return;
    }

    static const auto     _backgroundDuration    = Config::Get()["NewLoadingScreens"]["BackgroundTimer"].GetFloat();
    static auto           _backgroundSecondsLeft = _backgroundDuration;
    static constexpr auto _backgroundTransitionDuration    = 2.5f;  // 2:30 seconds
    static auto           _backgroundTransitionSecondsLeft = _backgroundTransitionDuration;
    static std::uint32_t  _currBackgroundImageIdx          = 0;
    static std::uint32_t  _nextBackgroundImageIdx          = 0;

    if (details::Hooks::resetTimers) {
      _backgroundSecondsLeft           = _backgroundDuration;
      _backgroundTransitionSecondsLeft = _backgroundTransitionDuration;
      _currBackgroundImageIdx          = _nextBackgroundImageIdx;

      details::Hooks::resetTimers = false;
    }

    if (details::Hooks::isShowingLoadingScreen) {
      ImGui::WithItemColor _color(ImGuiCol_WindowBg, {0.0f, 0.0f, 0.0f, 1.0f});
      ImGui::WithItemStyle _style1(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
      ImGui::WithItemStyle _style2(ImGuiStyleVar_Alpha, 1.0f);
      ImGui::SetNextWindowBgAlpha(1.0f);
      ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
      ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
      if (ImGui::Begin("###NewLoadingScreens", nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration)) {
        ImGui::Image(details::vTextures[_currBackgroundImageIdx], ImGui::GetWindowSize(), {0.0f, 0.0f}, {1.0f, 1.0f},
                     {1.0f, 1.0f, 1.0f, _backgroundTransitionSecondsLeft / _backgroundTransitionDuration});

        _backgroundSecondsLeft -= ImGui::GetIO().DeltaTime;
        if (_backgroundSecondsLeft <= 0.0f && details::vTextures.size() > 1) {
          while (_currBackgroundImageIdx == _nextBackgroundImageIdx)
            _nextBackgroundImageIdx = Random::Get<std::uint32_t>(0, details::vTextures.size() - 1);

          ImGui::SetCursorPos({0.0f, 0.0f});
          ImGui::Image(
              details::vTextures[_nextBackgroundImageIdx], ImGui::GetWindowSize(), {0.0f, 0.0f}, {1.0f, 1.0f},
              {1.0f, 1.0f, 1.0f,
               ((_backgroundTransitionDuration - _backgroundTransitionSecondsLeft) / _backgroundTransitionDuration)});

          _backgroundTransitionSecondsLeft -= ImGui::GetIO().DeltaTime;
          if (_backgroundTransitionSecondsLeft <= 0.0f) {
            _currBackgroundImageIdx          = _nextBackgroundImageIdx;
            _backgroundSecondsLeft           = _backgroundDuration;
            _backgroundTransitionSecondsLeft = _backgroundTransitionDuration;
          }
        }

        static const ImVec2 _loadingTextSizeActual = {static_cast<float>(details::loadingTextSprite.mWidth),
                                                      static_cast<float>(details::loadingTextSprite.mHeight)};
        static const auto   _loadingTextSizeRender =
            _loadingTextSizeActual / details::loadingTextSprite.mRenderSizeFactor;
        static const auto _loadingTextMaxFrame = details::loadingTextSprite.mFrames;

        static std::uint32_t _loadingTextFrame     = 0;
        static auto          _loadingTextFrameStep = 0.0f;

        _loadingTextFrameStep += _loadingTextMaxFrame / ImGui::GetIO().Framerate;
        if (_loadingTextFrameStep >= 1.0f) {
          _loadingTextFrame++;
          _loadingTextFrameStep = 0.0f;
        }
        if (_loadingTextFrame == _loadingTextMaxFrame) _loadingTextFrame = 0;

        ImGui::SetCursorPos(
            {ImGui::GetWindowWidth() - _loadingTextSizeRender.x, ImGui::GetWindowHeight() - _loadingTextSizeRender.y});
        ImGui::Image(details::loadingTextSprite.mSprites[_loadingTextFrame], _loadingTextSizeRender);
      }
      ImGui::End();
    }
  }

  static void Init() {
    Log(LogLevel::Debug, "Installing hooks.");
    {
      MemoryEditor::Get().Make(MemoryEditor::MakeType::Call, MemoryEditor::Get().AbsRVA(0x897A6),
                               reinterpret_cast<std::uintptr_t>(&details::Hooks::hkLoadingScreen_LoadScreen));
      MemoryEditor::Get().Make(MemoryEditor::MakeType::Jump, MemoryEditor::Get().AbsRVA(0x896FE),
                               reinterpret_cast<std::uintptr_t>(&details::Hooks::hkLoadingScreen_UnloadScreen));
    }
  }
}  // namespace Extensions::D3D9::NewLoadingScreens
