#include "Helpers.h"

int main() {
    HWND hwnd = FindWindowA(NULL, "Garry's Mod");
    DWORD proc_id; GetWindowThreadProcessId(hwnd, &proc_id);
    process_handle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, false, proc_id);

    MODULEENTRY32 client = get_module("client.dll", proc_id);
    auto bytes = new uint8_t[client.modBaseSize];
    DWORD bytes_read;
    ReadProcessMemory(process_handle, client.modBaseAddr, bytes, client.modBaseSize, &bytes_read);
    if (bytes_read != client.modBaseSize) throw;

    uintptr_t LocalPlayer = find_pattern(client, bytes, "3B 3D ? ? ? ? 75 48 8B 07", 0x2, 0x0);
    uintptr_t ForceJump = find_pattern(client, bytes, "8B 1D ? ? ? ? 8B D7", 0x2, -0xC);
    delete[] bytes;


    uintptr_t dwLocalPlayer = RPM<uintptr_t>(LocalPlayer);
    
    Bhop(dwLocalPlayer, ForceJump,m_Flags);

    system("pause");
    return 0;
}