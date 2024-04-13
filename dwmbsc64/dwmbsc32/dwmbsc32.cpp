#pragma once
#define WIN32_LEAN_AND_MEAN
#define HK_ID 1
#include <Windows.h>
#include <dwmapi.h>

typedef void (*SetHookFunc)();
typedef void (*RemoveHookFunc)();

BOOL CALLBACK PolWinProc(HWND hwnd, LPARAM lParam) {
    //politica de renderizado y transisiones
    DWMNCRENDERINGPOLICY NCRP = DWMNCRP_DISABLED;
    BOOL lpol = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &NCRP, sizeof(NCRP));
    DwmSetWindowAttribute(hwnd, DWMWA_TRANSITIONS_FORCEDISABLED, &lpol, sizeof(lpol));
    return TRUE;
}

BOOL CALLBACK dPolWinProc(HWND hwnd, LPARAM lParam) {
    //politica de renderizado y transisiones
    DWMNCRENDERINGPOLICY rNCRP = DWMNCRP_ENABLED;
    BOOL tpol = FALSE;
    DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &rNCRP, sizeof(rNCRP));
    DwmSetWindowAttribute(hwnd, DWMWA_TRANSITIONS_FORCEDISABLED, &tpol, sizeof(tpol));
    return TRUE;
}


int main() {

    HANDLE hMutex = CreateMutex(NULL, TRUE, "dwmbsc32");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        //Ya hay una instancia
        CloseHandle(hMutex);
        MessageBox(0, "Ya hay una instancia de dwmbsc ejecutandose", "Error", MB_ICONERROR | MB_OK);
        ExitProcess(NULL);
    }

    //ocultar consola
    FreeConsole();

    // Enumerar todas las ventanas existentes y aplicar politicas
    EnumWindows(PolWinProc, NULL);

    HINSTANCE hDll = LoadLibrary("hook.dll");
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
            case WM_HOTKEY:
                {
                if(msg.wParam == HK_ID) {

                // Enumerar todas las ventanas existentes y restaurar politicas
                EnumWindows(dPolWinProc, NULL);

                //remover gancho
                removeHook();

                //liberar la DLL
                FreeLibrary(hDll);

                // Desregistra el atajo de teclado al salir
                UnregisterHotKey(NULL, HK_ID);

                // matar proceso
                TerminateProcess(pi.hProcess, 0);

                // Cerrar los manejadores
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                return 0;

                }
                break;
        } //end switch
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
