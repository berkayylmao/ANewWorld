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
#include <future>

#pragma warning(push, 0)
// D3D9
#include <d3d9.h>
#include <d3dx9.h>

// Dear ImGui
#include "Helpers/Dear ImGui/imgui.h"
#include "Helpers/Dear ImGui/imgui_internal.h"  // math operators for ImVec2
#pragma warning(pop)

// ReSharper disable CppClangTidyClangDiagnosticOldStyleCast

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
    } // namespace Hooks

    struct Backgrounds {
      class BackgroundInfo {
        ImVec2 mFocalPoint;
        ImVec2 mUV0, mUV1;

        public:
          std::filesystem::path mPath;
          LPDIRECT3DTEXTURE9    mTexture{nullptr};
          ImVec2                mSize;

          void Draw(const ImVec4& tintColour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)) const {
            ImGui::Image(mTexture, ImGui::GetIO().DisplaySize, mUV0, mUV1, tintColour);
          }

          void CalculateImageUVs() {
            const auto& _displaySize = ImGui::GetIO().DisplaySize;
            if (_displaySize.x > mSize.x && _displaySize.y > mSize.y) {
              mUV0 = {0.0f, 0.0f};
              mUV1 = {1.0f, 1.0f};
            } else {
              const auto _displaySizeCenter = _displaySize / 2.0f;

              auto _imgLeft   = mFocalPoint.x - _displaySizeCenter.x;
              auto _imgTop    = mFocalPoint.y - _displaySizeCenter.y;
              auto _imgRight  = mFocalPoint.x + _displaySizeCenter.x;
              auto _imgBottom = mFocalPoint.y + _displaySizeCenter.y;

              if (_imgLeft < 0.0f) {
                _imgRight += -_imgLeft;
                _imgLeft = 0.0f;
              }
              if (_imgTop < 0.0f) {
                _imgBottom += -_imgTop;
                _imgTop = 0.0f;
              }

              while (_imgLeft >= _displaySizeCenter.x && _imgTop >= _displaySizeCenter.y &&
                     (_imgRight + _displaySizeCenter.x) <= mSize.x && (_imgBottom + _displaySizeCenter.y) <= mSize.y) {
                _imgLeft -= _displaySizeCenter.x;
                _imgTop -= _displaySizeCenter.y;
                _imgRight += _displaySizeCenter.x;
                _imgBottom += _displaySizeCenter.y;
              }

              mUV0 = {std::max(0.0f, _imgLeft / mSize.x), std::max(0.0f, _imgTop / mSize.y)};
              mUV1 = {std::min(1.0f, _imgRight / mSize.x), std::min(1.0f, _imgBottom / mSize.y)};
            }
          }

          explicit BackgroundInfo(const std::filesystem::path& path, const ImVec2& size, const ImVec2& focalPoint) {
            this->mPath       = path;
            this->mSize       = size;
            this->mFocalPoint = focalPoint;
          }
      };

      std::filesystem::path       mPath;
      std::vector<BackgroundInfo> mBackgrounds;

      explicit Backgrounds() {
        this->mPath          = Config::Get().GetConfigFolderPath() / "NewLoadingScreens" / "Backgrounds";
        const auto _infoJson = mPath / "info.json";

        try {
          if (!std::filesystem::exists(_infoJson)) {
            ::MessageBoxA(nullptr, "'info.json' is missing (for 'Backgrounds')!", "Error loading 'Backgrounds'!",
                          MB_ICONERROR | MB_OK);
          } else {
            rapidjson::Document _doc;

            std::ifstream             _ifs(_infoJson, std::ios_base::binary);
            rapidjson::IStreamWrapper _isw(_ifs);
            _doc.ParseStream(_isw);

            for (const auto& _info : _doc["Backgrounds"].GetArray()) {
              BackgroundInfo _bgInfo(
                  mPath / _info["FileName"].GetString(),
                  {static_cast<float>(_info["Width"].GetUint()), static_cast<float>(_info["Height"].GetUint())},
                  {static_cast<float>(_info["FocalPointX"].GetUint()),
                   static_cast<float>(_info["FocalPointY"].GetUint())});

              this->mBackgrounds.push_back(_bgInfo);
            }
          }
        } catch (const std::runtime_error& e) {
          ::MessageBoxA(nullptr, e.what(), "Error loading 'Backgrounds'!", MB_ICONERROR | MB_OK);
        }
      }
    };

    class LoadingTextSprite {
      ImVec2 mPivot{1.0f, 1.0f}, mOffset{0.0f, 0.0f};
      float  mPauseAfterLoop = 0.0f;

      public:
        std::filesystem::path           mPath;
        std::vector<LPDIRECT3DTEXTURE9> mSprites;
        std::uint32_t                   mTotalFrames{0}, mCurrentFrame{0};
        ImVec2                          mSize;

        void Draw() {
          const auto& _displaySize          = ImGui::GetIO().DisplaySize;
          static auto _loadingTextFrameStep = 0.0f;

          if (mCurrentFrame < mTotalFrames - 1) {
            _loadingTextFrameStep += static_cast<float>(mTotalFrames) / ImGui::GetIO().Framerate;
            if (_loadingTextFrameStep >= 1.0f) {
              mCurrentFrame++;
              _loadingTextFrameStep = 0.0f;
            }
          } else {
            _loadingTextFrameStep += ImGui::GetIO().DeltaTime;
            if (_loadingTextFrameStep >= mPauseAfterLoop) mCurrentFrame = 0;
          }

          ImGui::SetCursorPos(
          {(_displaySize.x * mPivot.x) - (mSize.x - mOffset.x),
           (_displaySize.y * mPivot.y) - (mSize.y - mOffset.y)});
          ImGui::Image(mSprites[mCurrentFrame], mSize);
        }

        explicit LoadingTextSprite() {
          this->mPath =
              Config::Get().GetConfigFolderPath() / "NewLoadingScreens" /
              fmt::format("LoadingText_{}", Config::Get()["NewLoadingScreens"]["LoadingTextResolution"].GetString());
          const auto _spriteJson = mPath / "sprite.json";

          try {
            if (!std::filesystem::exists(_spriteJson)) {
              ::MessageBoxA(nullptr, "Sprite files are missing (for 'LoadingText')!", "Error loading 'LoadingText'!",
                            MB_ICONERROR | MB_OK);
            } else {
              rapidjson::Document _doc;

              std::ifstream             _ifs(_spriteJson, std::ios_base::binary);
              rapidjson::IStreamWrapper _isw(_ifs);
              _doc.ParseStream(_isw);

              this->mTotalFrames    = _doc["Frames"].GetUint();
              this->mPauseAfterLoop = _doc["PauseAfterLoop"].GetFloat();
              this->mSize           =
                  ImVec2{static_cast<float>(_doc["Width"].GetUint()), static_cast<float>(_doc["Height"].GetUint())} *
                  _doc["RenderSizeFactor"].GetFloat();
              this->mPivot  = {_doc["PivotX"].GetFloat(), _doc["PivotY"].GetFloat()};
              this->mOffset = {_doc["OffsetX"].GetFloat(), _doc["OffsetY"].GetFloat()};

              this->mSprites.resize(mTotalFrames);
            }
          } catch (const std::runtime_error& e) {
            ::MessageBoxA(nullptr, e.what(), "Error loading sprite (for 'LoadingText')!", MB_ICONERROR | MB_OK);
          }
        }

        ~LoadingTextSprite() = default;
    };

    static inline std::mutex createTexturesMutex;

    static inline bool forceIgnoreFeature      = false;
    static inline bool alreadyCreatingTextures = false;
    static inline bool texturesCreated         = false;

    static inline Backgrounds       backgrounds;
    static inline LoadingTextSprite loadingTextSprite;

    static void CreateTextures(LPDIRECT3DDEVICE9 pDevice) {
      std::scoped_lock<std::mutex> _lock(createTexturesMutex);
      if (alreadyCreatingTextures || texturesCreated) return;

      Log(LogLevel::Info, "Creating textures...");
      alreadyCreatingTextures = true;

      HRESULT _result;

      Log(LogLevel::Info, "Creating Backgrounds textures...");
      {
        Log(LogLevel::Debug, fmt::format("Textures folder: {}", backgrounds.mPath.u8string()));

        for (auto& background : backgrounds.mBackgrounds) {
          Log(LogLevel::Debug, fmt::format("Processing '{}'", background.mPath.u8string()));

          if (FAILED(_result = D3DXCreateTextureFromFileExW(pDevice, background.mPath.wstring().c_str(),
            static_cast<UINT>(background.mSize.x),
            static_cast<UINT>(background.mSize.y), 1, 0, D3DFMT_UNKNOWN,
            D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER,
            D3DX_DEFAULT, 0, nullptr, nullptr, &background.mTexture)))
            Log(LogLevel::Error, fmt::format("Texture couldn't be created.\n\tResult:\n\tFile:{}\n\t",
                background.mPath.u8string(), _result));
          else
            background.CalculateImageUVs();
        }
      }

      Log(LogLevel::Info, "Creating LoadingText texture...");
      {
        Log(LogLevel::Debug, fmt::format("Textures folder: {}", loadingTextSprite.mPath.u8string()));

        for (std::size_t i = 0; i < loadingTextSprite.mTotalFrames; i++) {
          const auto _file = loadingTextSprite.mPath / (std::to_string(i) + ".png");

          if (FAILED(_result = D3DXCreateTextureFromFileExW(
            pDevice, _file.wstring().c_str(), static_cast<UINT>(loadingTextSprite.mSize.x),
            static_cast<UINT>(loadingTextSprite.mSize.y), 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
            D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER, D3DX_DEFAULT, 0, nullptr, nullptr,
            &loadingTextSprite.mSprites[i])))
            Log(LogLevel::Error,
              fmt::format("Texture couldn't be created.\n\tResult:\n\tFile:{}\n\t", _file.u8string(), _result));
        }
      }

      texturesCreated         = true;
      alreadyCreatingTextures = false;
    }

    static LRESULT CALLBACK WndProc(HWND, UINT uMsg, WPARAM wParam, LPARAM) {
      if (uMsg == WM_SYSKEYDOWN && wParam == 0x5A) forceIgnoreFeature = !forceIgnoreFeature;
      return MirrorHook::WndProc::g_constIgnoreThisReturn;
    }
  } // namespace details

  static void Draw(LPDIRECT3DDEVICE9 pDevice) {
    if (details::forceIgnoreFeature || !Config::Get()["NewLoadingScreens"]["Enabled"].GetBool()) return;
    if (!details::texturesCreated) {
      if (!details::alreadyCreatingTextures)
        auto _dummy = std::async(std::launch::async, details::CreateTextures, pDevice);

      return;
    }

    const auto            _backgroundDuration = Config::Get()["NewLoadingScreens"]["BackgroundTimer"].GetFloat();
    static auto           _backgroundSecondsLeft = _backgroundDuration;
    static constexpr auto _backgroundTransitionDuration = 2.5f; // 2:30 seconds
    static auto           _backgroundTransitionSecondsLeft = _backgroundTransitionDuration;
    static std::uint32_t  _currBackgroundImageIdx = 0;
    static std::uint32_t  _nextBackgroundImageIdx = 0;

    if (details::Hooks::resetTimers) {
      _backgroundSecondsLeft           = _backgroundDuration;
      _backgroundTransitionSecondsLeft = _backgroundTransitionDuration;
      _currBackgroundImageIdx          = _nextBackgroundImageIdx;

      details::Hooks::resetTimers = false;
    }

#ifdef _DEBUG
    if constexpr (true) {
#else
    if (details::Hooks::isShowingLoadingScreen) {
#endif
      ImGui::WithItemColor _color(ImGuiCol_WindowBg, {0.0f, 0.0f, 0.0f, 1.0f});
      ImGui::WithItemStyle _style1(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
      ImGui::WithItemStyle _style2(ImGuiStyleVar_Alpha, 1.0f);
      ImGui::SetNextWindowBgAlpha(1.0f);
      ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
      ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
      if (ImGui::Begin("###NewLoadingScreens", nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration)) {
        // Background
        {
          details::backgrounds.mBackgrounds[_currBackgroundImageIdx].Draw(
              {1.0f, 1.0f, 1.0f, _backgroundTransitionSecondsLeft / _backgroundTransitionDuration});

          if (details::backgrounds.mBackgrounds.size() > 1) _backgroundSecondsLeft -= ImGui::GetIO().DeltaTime;

          if (_backgroundSecondsLeft <= 0.0f) {
            while (_currBackgroundImageIdx == _nextBackgroundImageIdx)
              _nextBackgroundImageIdx = Random::Get<std::uint32_t>(0, details::backgrounds.mBackgrounds.size() - 1);

            ImGui::SetCursorPos({0.0f, 0.0f});
            details::backgrounds.mBackgrounds[_nextBackgroundImageIdx].Draw(
            {1.0f,
             1.0f,
             1.0f,
             (_backgroundTransitionDuration - _backgroundTransitionSecondsLeft) / _backgroundTransitionDuration});

            _backgroundTransitionSecondsLeft -= ImGui::GetIO().DeltaTime;
            if (_backgroundTransitionSecondsLeft <= 0.0f) {
              _currBackgroundImageIdx          = _nextBackgroundImageIdx;
              _backgroundSecondsLeft           = _backgroundDuration;
              _backgroundTransitionSecondsLeft = _backgroundTransitionDuration;
            }
          }
        }

        // LoadingText
        {
          details::loadingTextSprite.Draw();
        }
      }
      ImGui::End();
    }
  }

  static void Init() {
    Log(LogLevel::Debug, "Installing hooks...");
    {
      MemoryEditor::Get().Make(MemoryEditor::MakeType::Call, MemoryEditor::Get().AbsRVA(0x897A6),
                               reinterpret_cast<std::uintptr_t>(&details::Hooks::hkLoadingScreen_LoadScreen));
      MemoryEditor::Get().Make(MemoryEditor::MakeType::Jump, MemoryEditor::Get().AbsRVA(0x896FE),
                               reinterpret_cast<std::uintptr_t>(&details::Hooks::hkLoadingScreen_UnloadScreen));
    }

    Log(LogLevel::Debug, "Adding MirrorHook (WndProc) extension...");
    {
      MirrorHook::WndProc::AddExtension(&details::WndProc);
    }
  }
} // namespace Extensions::D3D9::NewLoadingScreens
