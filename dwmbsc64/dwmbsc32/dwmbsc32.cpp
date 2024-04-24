#pragma once
#define TRAY_ICON_ID 1
#define HK_ID 2
#define TRAY_EXIT 3
#define WM_TRAY_ICON 1000
#include <Windows.h>
#include <shellapi.h>
#include <dwmapi.h>

typedef void (*SetHookFunc)();
typedef void (*RemoveHookFunc)();

void _PoL(HWND hwnd,const char* Option)
{
        //politica de renderizado y transisiones
    if (strcmp(Option, "Enable") == 0)
    {
        DWMNCRENDERINGPOLICY rNCRP = DWMNCRP_ENABLED;
        BOOL tpol = FALSE;
        DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &rNCRP, sizeof(rNCRP));
        DwmSetWindowAttribute(hwnd, DWMWA_TRANSITIONS_FORCEDISABLED, &tpol, sizeof(tpol));
    }
    else if (strcmp(Option, "Disable") == 0)
    {
        DWMNCRENDERINGPOLICY NCRP = DWMNCRP_DISABLED;
        BOOL lpol = TRUE;
        DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &NCRP, sizeof(NCRP));
        DwmSetWindowAttribute(hwnd, DWMWA_TRANSITIONS_FORCEDISABLED, &lpol, sizeof(lpol));
    }
}


BOOL CALLBACK PolWinProc(HWND hwnd, LPARAM lParam) {
    _PoL(hwnd, "Disable");

    //exclude explorer
    WCHAR className[32]; // WCHAR en lugar de TCHAR para manejar cadenas amplias
    GetClassNameW(hwnd, className, sizeof(className)/sizeof(className[0])); // Usar GetClassNameW para obtener el nombre de clase

    // Compara el nombre de clase con el de la ventana de Explorer
    if (wcscmp(className, L"CabinetWClass") == 0) { // Usar wcscmp para comparar cadenas amplias
        _PoL(hwnd, "Enable"); // Habilitar la política
    }
    return TRUE;
}

BOOL CALLBACK dPolWinProc(HWND hwnd, LPARAM lParam) {
    _PoL(hwnd, "Enable");
    return TRUE;
}

void ShowMenu(HWND hWnd)
{
    HMENU hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, TRAY_EXIT, "Exit");

    POINT pt;
    GetCursorPos(&pt);
    TrackPopupMenu(hMenu, TPM_RIGHTALIGN | TPM_BOTTOMALIGN | TPM_NONOTIFY, pt.x, pt.y, 0, hWnd, NULL);
    DestroyMenu(hMenu);
}


int main() {

    //ocultar consola
    FreeConsole();

    //ventana invisible
    HWND hWnd = CreateWindowEx(0, "Message", "dwmbsc", 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL);

    //cargar estructura
    NOTIFYICONDATA nid;
    ZeroMemory(&nid, sizeof(NOTIFYICONDATA));

    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd; // se necesita una ventana
    nid.uID = TRAY_ICON_ID;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAY_ICON;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    strcpy(nid.szTip, "dwmbsc");

    //agregar trayicon
    Shell_NotifyIcon(NIM_ADD, &nid);

    HANDLE hMutex = CreateMutex(NULL, TRUE, "dwmbsc32");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        //Ya hay una instancia
        CloseHandle(hMutex);
        MessageBox(0, "Ya hay una instancia de dwmbsc ejecutandose", "Error", MB_ICONERROR | MB_OK);
        ExitProcess(NULL);
    }

    // Enumerar todas las ventanas existentes y aplicar politicas
    EnumWindows(PolWinProc, NULL);

    HINSTANCE hDll = LoadLibrary("dwmbsc32.dll");
    if (hDll == NULL) {
        MessageBox(NULL, "Failed to load DLL 32", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Obtener puntero a para registrar el gancho de 32 bits
    SetHookFunc setHook = (SetHookFunc)GetProcAddress(hDll, "SetHook");
    RemoveHookFunc removeHook = (RemoveHookFunc)GetProcAddress(hDll, "RemoveHook");
    if (setHook == NULL || removeHook == NULL) {
        MessageBox(NULL, "Failed to get function pointers 32", "Error", MB_OK | MB_ICONERROR);
        FreeLibrary(hDll);
        return 1;
    }

    //instalar el gancho
    setHook();

    //informacion del proceso resultante
    PROCESS_INFORMATION pi = {0};
    STARTUPINFO si = {0};
    si.cb = sizeof(si);

    //crear subproceso de dwmbsc64
    if (!CreateProcess(
            "dwmbsc64.exe",  // Ruta al ejecutable
            NULL,                         // Argumentos de la línea de comandos
            NULL,                         // Atributos del proceso (predeterminado)
            NULL,                         // Atributos del hilo (predeterminado)
            FALSE,                        // Herencia de manijas (falso para no heredar)
            CREATE_NO_WINDOW,            // Crear sin ventana de consola
            NULL,                         // Medio ambiente del proceso (predeterminado)
            NULL,                         // Directorio base (predeterminado)
            &si,                          // Información de inicio del proceso
            &pi)) {                       // Información del proceso resultante
            return 1;
    }

    //registrar HotKey
    RegisterHotKey(NULL, HK_ID, MOD_CONTROL | MOD_SHIFT, VK_F1);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        switch (msg.message)
        {
            case WM_HOTKEY:
                {
                    switch(msg.wParam)
                    {
                        case HK_ID:
                        {
                            PostMessage(NULL, WM_CLOSE, 0, 0);
                        }
                        break;
                        case TRAY_EXIT:
                        {
                            PostMessage(NULL, WM_CLOSE, 0, 0);
                        }
                        break;
                    } //end switch
                } //end switch
                break;
            case WM_TRAY_ICON:
                {
                if (msg.lParam == WM_RBUTTONDOWN && msg.wParam == TRAY_ICON_ID)
                    {
                        ShowMenu(hWnd);
                    }
                }
                break;
            case WM_CLOSE:
                {
                    // Enumerar todas las ventanas existentes y restaurar politicas
                    EnumWindows(dPolWinProc, NULL);

                    //remover gancho
                    removeHook();

                    //liberar la DLL
                    FreeLibrary(hDll);

                    // remover atajo
                    UnregisterHotKey(NULL, HK_ID);

                    // matar proceso hijo
                    TerminateProcess(pi.hProcess, 0);

                    // eliminar trayicon
                    Shell_NotifyIcon(NIM_DELETE, &nid);

                    // eliminar ventana invisible
                    DestroyWindow(hWnd);

                    // Cerrar los manejadores
                    CloseHandle(pi.hProcess);
                    CloseHandle(pi.hThread);

                    //salir
                    return 0;
                }
                break;
        } //end switch
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
