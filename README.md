# dwmbsc
el basic themer más ligero y completo

![image](https://github.com/LuSlower/dwmbsc/assets/148411728/61e9273f-1c22-4144-a905-4a301310bc2f)

en lugar de sondear a GetForegroundWindow() como la mayoria, pensé en un SHELL HOOK ya que no tendría que sondear esa función cada Sleep(_n_) hasta que detecte una ventana en primer plano, incluso definiendo un hWndPrev

tal vez te preguntes por que hay dos ejecutables?

bien, esto tiene una explicación
para poder inyectar correctamente el ShellProc, este requiere de dos ejecutables que llamen a los enlaces para las arquitecturas (32 y 64 bits) de diferentes programas

MSDN dice:

> SetWindowsHookEx se puede utilizar para inyectar una DLL en otro proceso. No se puede inyectar una DLL de 32 bits en un proceso de 64 bits y no se puede inyectar una DLL de 64 bits en un proceso de 32 bits. Si una aplicación requiere el uso de enlaces en otros procesos, se requiere que una aplicación de 32 bits llame a SetWindowsHookEx para inyectar una DLL de 32 bits en procesos de 32 bits, y una aplicación de 64 bits llame a SetWindowsHookEx para inyectar una DLL de 64 bits. DLL en procesos de 64 bits. Las DLL de 32 y 64 bits deben tener nombres diferentes.


y bueno esto generaria un descanso a la CPU, ya que evitaria sondear constantemente a esta función
y Sleep no es algo en lo que se deba confiar en Windows

> Debido a que los ganchos se ejecutan en el contexto de una aplicación, deben coincidir con el "bitness" de la aplicación. Si una aplicación de 32 bits instala un enlace global en Windows de 64 bits, el enlace de 32 bits se inyecta en cada proceso de 32 bits (se aplican los límites de seguridad habituales). En un proceso de 64 bits, los subprocesos todavía están marcados como "enganchados". Sin embargo, debido a que una aplicación de 32 bits debe ejecutar el código de enlace, el sistema ejecuta el enlace en el contexto de la aplicación de enlace; específicamente, en el hilo que llamó SetWindowsHookEx . Esto significa que la aplicación de enlace debe continuar enviando mensajes o podría bloquear el funcionamiento normal de los procesos de 64 bits.

## en que se diferencia de basicthemer5?

* 13MB aproximadamente de total bytes (basicthemer5) frente a 2.5MB (entre los dos ejecutables de dwmbsc)
* no hay sondeo de funciones, solo hay un while en cada proceso para evitar que el ShellProc bloquee su sistema
* cada ejecutable depende de cada uno de los DLLs para que pueda inyectar correctamente el HOOKPROC a cada proceso dependiendo del contexto de su arquitectura
* añadi un hootkey a dwmbsc32.exe (CTRL + SHIFT + F1) esto restaurara las politicas aplicadas a cada atributo de dwm

## INFO

* si no desea usar el SysMenu de Explorer, Chrome / Edge, puede usar este [tema](https://chromewebstore.google.com/detail/windows-vista-basic-theme/bkohfcingfpclphbaglfbbjbfajcepad) 
para activar el tema heredado en Chrome / Edge, necesita inicializar el programa con **-disable-windows10-custom-titleba** para eso mejor cree un acceso directo

## COMO USAR?

* abre dwmbsc32.exe, este a su vez abrira el otro ejecutable (dwmbsc64.exe) como un proceso hijo, cuando presiones el hootkey automaticamente se cerrara junto a su proceso padre

* **DISFRUTAR**

## DERECHOS DE AUTOR Y LICENCIA
[LICENCIA](LICENSE)
