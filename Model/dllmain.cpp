﻿#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <Model/ModelAPI.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            break;

        default:
            return FALSE;
            break;
    }
    return TRUE;
}
