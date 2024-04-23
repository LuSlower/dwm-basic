#include <stdio.h>
#include <windows.h>
#include <dwmapi.h>
#define HK_ID 1

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

HWND HWNDPrev = NULL; // Declaración de HWNDPrev antes de WinEventProc
WCHAR className[256]; // Declarar tamaño de clase

void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
    if (event == EVENT_OBJECT_CREATE && hwnd != NULL && hwnd != HWNDPrev)
    {
        if (GetClassNameW(hwnd, className, sizeof(className)/sizeof(className[0])) > 0)
        {
            // Comprobar si la ventana actual es la ventana de Explorer
            if (wcscmp(className, L"CabinetWClass") == 0)
            {
                return;
            }
        }

        // Deshabilitar la política en la ventana anterior
            _PoL(hwnd, "Disable");

        // Actualizar HWNDPrev con el valor de la ventana actual
        HWNDPrev = hwnd;
    }
    else
    {
        return;
    }

}


int main()
{

    HANDLE hMutex = CreateMutex(NULL, TRUE, "dwmbsc32");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        //Ya hay una instancia
        CloseHandle(hMutex);
        MessageBox(0, "Ya hay una instancia de dwm-bs ejecutandose", "Error", MB_ICONERROR | MB_OK);
        ExitProcess(NULL);
    }

    FreeConsole();

    // Enumerar todas las ventanas existentes y aplicar politicas
    EnumWindows(PolWinProc, NULL);

    //registrar HotKey
    RegisterHotKey(NULL, HK_ID, MOD_CONTROL | MOD_SHIFT, VK_F1);

    // Registrar el evento EVENT_OBJECT_CREATE
    HWINEVENTHOOK hHook = SetWinEventHook(EVENT_OBJECT_CREATE, EVENT_OBJECT_CREATE, NULL, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT);

    if (hHook == NULL)
    {
        fprintf(stderr, "Failed to set event hook.\n");
        return 1;
    }

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
                    } //end switch
                } //end switch
                break;
            case WM_CLOSE:
                {
                    // Enumerar todas las ventanas existentes y restaurar politicas
                    EnumWindows(dPolWinProc, NULL);

                    //remover gancho
                    UnhookWinEvent(hHook);

                    // remover atajo
                    UnregisterHotKey(NULL, HK_ID);

                    CloseHandle(hMutex);

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
