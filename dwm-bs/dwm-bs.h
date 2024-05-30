#pragma once
#include <windows.h>
#include <dwmapi.h>
#include <stdio.h>
#include <tlhelp32.h>
#define HK_ID 1

HWND HWNDPrev = NULL;
WCHAR className[32];
HWINEVENTHOOK hHook;
BOOL lpol, tpol;


DWORD GetPID(const char* processName) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        printf("Error al crear un snapshot de procesos");
        return 0;
    }

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(snapshot, &entry)) {
        CloseHandle(snapshot);
        printf("Error al obtener la primera entrada de proceso");
        return 0;
    }

    DWORD processId = 0;
    do {
        if (strcmp(entry.szExeFile, processName) == 0) {
            processId = entry.th32ProcessID;
            break;
        }
    } while (Process32Next(snapshot, &entry));

    CloseHandle(snapshot);
    return processId;
}

const char* GetExcludeList()
{
    static char pBuffer[64];
    DWORD pSize = sizeof(pBuffer);

    // Leer la lista de procesos excluidos
    HKEY hKey;
    const char* lpSubKey = "Software\\dwm-bs";
    DWORD valueType = REG_MULTI_SZ;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        // Crear la clave por si no existe
        RegCreateKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hKey, NULL);
    }

    if (RegQueryValueExA(hKey, "ExclusionList", NULL, &valueType, (LPBYTE)pBuffer, &pSize) != ERROR_SUCCESS){
        const char* initialValue = "explorer.exe\0\0";
        RegSetValueExA(hKey, "ExclusionList", 0, REG_MULTI_SZ, (const BYTE*)initialValue, strlen(initialValue));
    }

    // Cerrar la clave
    RegCloseKey(hKey);

    return pBuffer;
}

void _PoL(HWND hwnd, const char* Option)
{
        //politica de renderizado y transisiones
    if (strcmp(Option, "Enable") == 0) {
        DWMNCRENDERINGPOLICY rNCRP = DWMNCRP_ENABLED;
        tpol = FALSE;
        DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &rNCRP, sizeof(rNCRP));
        DwmSetWindowAttribute(hwnd, DWMWA_TRANSITIONS_FORCEDISABLED, &tpol, sizeof(tpol));
    } else if (strcmp(Option, "Disable") == 0) {
        DWMNCRENDERINGPOLICY NCRP = DWMNCRP_DISABLED;
        lpol = TRUE;
        DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &NCRP, sizeof(NCRP));
        DwmSetWindowAttribute(hwnd, DWMWA_TRANSITIONS_FORCEDISABLED, &lpol, sizeof(lpol));
    }
}

void _DisableBlurBehind(HWND hWnd)
{
        //politica de transparencia
        DWM_BLURBEHIND bb = {0};
        bb.dwFlags = DWM_BB_ENABLE;
        bb.fEnable = FALSE;
        bb.hRgnBlur = NULL;

        DwmEnableBlurBehindWindow(hWnd, &bb);
}

BOOL IsExcludeHWND(HWND hWnd_ex)
{
    DWORD pid;
    GetWindowThreadProcessId(hWnd_ex, &pid);

    const char* pBuffer = GetExcludeList();

    // Verificar si el proceso de la ventana está en la lista de exclusión
    while (*pBuffer) {
        DWORD pid_ex = GetPID(pBuffer);
        if (pid == pid_ex) {
            return TRUE;
        }
        pBuffer += strlen(pBuffer) + 1;
    }

    return FALSE;
}

BOOL CALLBACK PolWinProc(HWND hwnd, LPARAM lParam) {
    _PoL(hwnd, "Disable");

        // Comprobar si la ventana es de un proceso excluido
        if (IsExcludeHWND(hwnd))
        {
            _PoL(hwnd, "Enable");
        }
    return TRUE;
}

BOOL CALLBACK dPolWinProc(HWND hwnd, LPARAM lParam) {
    _PoL(hwnd, "Enable");
    return TRUE;
}

void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
    if (event == EVENT_SYSTEM_FOREGROUND && hwnd != HWNDPrev && hwnd != NULL)
    {
            // Comprobar si la ventana actual es de un proceso excluido
            if (IsExcludeHWND(hwnd))
            {
                return;
            }

        // Deshabilitar la política en la ventana anterior
            _PoL(hwnd, "Disable");
            _DisableBlurBehind(hwnd);

        // Actualizar HWNDPrev con el valor de la ventana actual
        HWNDPrev = hwnd;
    }
    else
    {
        return;
    }

}

