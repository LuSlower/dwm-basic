#include "dwm-bs.h"

int main()
{

    FreeConsole();

    // Enumerar todas las ventanas existentes y aplicar politicas
    EnumWindows(PolWinProc, NULL);

    // Registrar HotKey
    RegisterHotKey(NULL, HK_ID, MOD_CONTROL | MOD_SHIFT, VK_F1);

    // Registrar el evento EVENT_SYSTEM_FOREGROUND
    hHook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS | WINEVENT_SKIPOWNTHREAD);

    // Prioridad de segundo plano
    SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_BEGIN);

    // Drenar algo de working set
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
