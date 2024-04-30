#include <Windows.h>

typedef void (*SetHookFunc)();
typedef void (*RemoveHookFunc)();

int main() {

    // ocultar shell cmd
    FreeConsole();

    // Cargar la DLL
    HINSTANCE hDll = LoadLibrary("dwmbsc64.dll");
    if (hDll == NULL) {
        MessageBox(NULL, "Failed to load DLL 64", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Obtener puntero a para registrar el gancho de 64 bits
    SetHookFunc setHook = (SetHookFunc)GetProcAddress(hDll, "SetHook");
    RemoveHookFunc removeHook = (RemoveHookFunc)GetProcAddress(hDll, "RemoveHook");
    if (setHook == NULL || removeHook == NULL) {
        MessageBox(NULL, "Failed to get function pointers 64", "Error", MB_OK | MB_ICONERROR);
        FreeLibrary(hDll);
        return 1;
    }

    // Instalar el gancho
    setHook();

    // Mantener el programa en ejecución
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //remover gancho
    removeHook();

    //liberar la DLL
    FreeLibrary(hDll);

    return 0;
}
