#include "pch.h"

void Init() {
#if defined(_DEBUG)
  std::FILE* _hFreOpen = nullptr;
  AllocConsole();
  freopen_s(&_hFreOpen, "CONOUT$", "w", stdout);
#endif

}

BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID) {
  if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
    DisableThreadLibraryCalls(hModule);
    std::thread(Init).detach();
  }
  return TRUE;
}
