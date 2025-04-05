#include "dwm-bs.h"

int main(int argc, char *argv[])
{
    if (argc == 2 && strcmp(argv[1],"-m") == 0 ){
        fEnableMMCSS = TRUE;
    }

    FreeConsole();

    EnumWindows(PolWinProc, NULL);

    RegisterHotKey(NULL, HK_ID, MOD_CONTROL | MOD_SHIFT, VK_F1);

    hHook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS | WINEVENT_SKIPOWNTHREAD);

    SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_BEGIN);

    SetProcessWorkingSetSize(GetCurrentProcess(), (SIZE_T) -1, (SIZE_T) -1);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        switch (msg.message)
        {
            case WM_HOTKEY:
                {
                    if (msg.wParam == HK_ID)
                        {
                            EnumWindows(dPolWinProc, NULL);

                            UnhookWinEvent(hHook);

                            UnregisterHotKey(NULL, HK_ID);

                            DwmEnableMMCSS(FALSE);

                            return 0;
                        }
                } 
                break;
        } 
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
