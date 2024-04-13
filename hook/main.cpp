#include <dwmapi.h>
#include "main.h"
// Declarar la sección compartida
HHOOK hhk = NULL;
HINSTANCE hInst = NULL;
LRESULT CALLBACK GetWinProc(int cCode, WPARAM wParam, LPARAM lParam) {
    if (cCode == HSHELL_WINDOWCREATED) {
        HWND hwnd = (HWND)wParam;
        //politica de renderizado y transisiones
        DWMNCRENDERINGPOLICY NCRP = DWMNCRP_DISABLED;
        BOOL lpol = TRUE;
        DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &NCRP, sizeof(NCRP));
        DwmSetWindowAttribute(hwnd, DWMWA_TRANSITIONS_FORCEDISABLED, &lpol, sizeof(lpol));
    }
    // Llama al siguiente gancho en la cadena
    return CallNextHookEx(hhk, cCode, wParam, lParam);
}



extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            hInst = (HINSTANCE) hinstDLL;
            // attach from process
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}

void SetHook()
{
    hhk = SetWindowsHookEx(WH_SHELL, (HOOKPROC)GetWinProc, hInst, 0);
        // Verificar si se estableció correctamente
    if (hhk == NULL)
    {
        MessageBox(0, "Failed to implement hook", "Failed", MB_OK | MB_ICONERROR);
    }
}

void RemoveHook()
{
    UnhookWindowsHookEx(hhk);
}


