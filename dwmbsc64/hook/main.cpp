#include <dwmapi.h>
#include "main.h"

//declarar DLLs
HHOOK hhk = NULL;
HINSTANCE hInst = NULL;

void _PoL(HWND hwnd, const char* Option)
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

//definir gancho
LRESULT CALLBACK GetWinProc(int cCode, WPARAM wParam, LPARAM lParam) {
    if (cCode == HSHELL_WINDOWCREATED) {
        HWND hwnd = (HWND)wParam;

    //Deshabilitar renderizado y transiciones
    _PoL(hwnd, "Disable");

    //exclude explorer
    WCHAR className[32]; // WCHAR en lugar de TCHAR para manejar cadenas amplias
    GetClassNameW(hwnd, className, sizeof(className)/sizeof(className[0])); // Usar GetClassNameW para obtener el nombre de clase

    // Compara el nombre de clase con el de la ventana de Explorer
    if (wcscmp(className, L"CabinetWClass") == 0) { // Usar wcscmp para comparar cadenas amplias
        _PoL(hwnd, "Enable"); // Habilitar la política
    }
    }
    // Llama al siguiente gancho en la cadena
    return CallNextHookEx(hhk, cCode, wParam, lParam);
}



extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach from process
            hInst = (HINSTANCE) hinstDLL;
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
    //instalar gancho
    hhk = SetWindowsHookEx(WH_SHELL, (HOOKPROC)GetWinProc, hInst, 0);
}

void RemoveHook()
{
    //remover gancho
    UnhookWindowsHookEx(hhk);
}


