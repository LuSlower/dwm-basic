#include <stdio.h>
#include <windows.h>
#include <dwmapi.h>
#define HK_ID 1

HWND HWNDPrev = NULL; // Declaración de HWNDPrev antes de WinEventProc
WCHAR className[32]; // Declarar tamaño de clase
HWINEVENTHOOK hHook; // Declarar WinEventHook
BOOL lpol, tpol; // Declarar switchs


void _PoL(HWND hwnd,const char* Option)
{
        //politica de renderizado y transisiones
    if (strcmp(Option, "Enable") == 0)
    {
        DWMNCRENDERINGPOLICY rNCRP = DWMNCRP_ENABLED;
        tpol = FALSE;
        DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &rNCRP, sizeof(rNCRP));
        DwmSetWindowAttribute(hwnd, DWMWA_TRANSITIONS_FORCEDISABLED, &tpol, sizeof(tpol));
    }
    else if (strcmp(Option, "Disable") == 0)
    {
        DWMNCRENDERINGPOLICY NCRP = DWMNCRP_DISABLED;
        lpol = TRUE;
        DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &NCRP, sizeof(NCRP));
        DwmSetWindowAttribute(hwnd, DWMWA_TRANSITIONS_FORCEDISABLED, &lpol, sizeof(lpol));
    }
}

BOOL CALLBACK PolWinProc(HWND hwnd, LPARAM lParam) {
    _PoL(hwnd, "Disable");

    if (GetClassNameW(hwnd, className, sizeof(className)/sizeof(className[0])) > 0)
    {
        // Comprobar si la ventana actual es la ventana de Explorer
        if (wcscmp(className, L"CabinetWClass") == 0)
        {
            _PoL(hwnd, "Enable");
        }
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

    FreeConsole();

    // Enumerar todas las ventanas existentes y aplicar politicas
    EnumWindows(PolWinProc, NULL);

    //registrar HotKey
    RegisterHotKey(NULL, HK_ID, MOD_CONTROL | MOD_SHIFT, VK_F1);

    // Registrar el evento EVENT_OBJECT_CREATE
    hHook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS | WINEVENT_SKIPOWNTHREAD);

    // Prioridad de segundo plano
    SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_BEGIN);

    // Drenar algo de WS
    SetProcessWorkingSetSize(GetCurrentProcess(), (SIZE_T) -1, (SIZE_T) -1);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        switch (msg.message)
        {
            case WM_HOTKEY:
                {
                    if (msg.wParam == HK_ID)
                        {
                            // Enumerar todas las ventanas existentes y restaurar politicas
                            EnumWindows(dPolWinProc, NULL);

                            //remover gancho
                            UnhookWinEvent(hHook);

                            // remover atajo
                            UnregisterHotKey(NULL, HK_ID);

                            return 0;
                        }
                } //end switch
                break;
        } //end switch
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //salir
    return 0;
}
