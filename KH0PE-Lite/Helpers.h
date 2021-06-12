#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "Bhop.h"
inline HANDLE process_handle;

inline int m_Flags = 0x350;

template<typename T> T RPM(SIZE_T address) {
    T buffer;
    ReadProcessMemory(process_handle, (LPCVOID)address, &buffer, sizeof(T), NULL);
    return buffer;
}

template<typename T> void WPM(SIZE_T address, T buffer) {
    WriteProcessMemory(process_handle, (LPVOID)address, &buffer, sizeof(buffer), NULL);
}

inline MODULEENTRY32 get_module(const char* modName, DWORD proc_id) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc_id);
    if (hSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry)) {
            do {

                if (!strcmp(modEntry.szModule, modName)) {
                    CloseHandle(hSnap);
                    return modEntry;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    MODULEENTRY32 module = { -1 };
    return module;
}

inline uintptr_t find_pattern(MODULEENTRY32 module, uint8_t* arr, const char* pattern, int offset, int extra) {
    uintptr_t scan = 0x0;
    const char* pat = pattern;
    uintptr_t firstMatch = 0;
    for (uintptr_t pCur = (uintptr_t)arr; pCur < (uintptr_t)arr + module.modBaseSize; ++pCur) {
        if (!*pat) { scan = firstMatch; break; }
        if (*(uint8_t*)pat == '\?' || *(uint8_t*)pCur == ((((pat[0] & (~0x20)) >= 'A' && (pat[0] & (~0x20)) <= 'F') ? ((pat[0] & (~0x20)) - 'A' + 0xa) : ((pat[0] >= '0' && pat[0] <= '9') ? pat[0] - '0' : 0)) << 4 | (((pat[1] & (~0x20)) >= 'A' && (pat[1] & (~0x20)) <= 'F') ? ((pat[1] & (~0x20)) - 'A' + 0xa) : ((pat[1] >= '0' && pat[1] <= '9') ? pat[1] - '0' : 0)))) {
            if (!firstMatch) firstMatch = pCur;
            if (!pat[2]) { scan = firstMatch; break; }
            if (*(WORD*)pat == 16191 || *(uint8_t*)pat != '\?') pat += 3;
            else pat += 2;
        }
        else { pat = pattern; firstMatch = 0; }
    }
    if (!scan) return 0x0;
    uint32_t read;
    ReadProcessMemory(process_handle, (void*)(scan - (uintptr_t)arr + (uintptr_t)module.modBaseAddr + offset), &read, sizeof(read), NULL);
    return read + extra;
}
