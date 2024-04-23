# dwmbsc
[![Total Downloads](https://img.shields.io/github/downloads/LuSlower/dwmbsc/total.svg)](https://github.com/LuSlower/dwmbsc/releases) [![PayPal Badge](https://img.shields.io/badge/PayPal-003087?logo=paypal&logoColor=fff&style=flat)](https://paypal.me/eldontweaks) 

el basic themer más ligero y completo

![image](https://github.com/LuSlower/dwmbsc/assets/148411728/61e9273f-1c22-4144-a905-4a301310bc2f)

en lugar de sondear a GetForegroundWindow() como los basic themers más antiguos, pensé que lo mejor sería un ShellHook ya que no tendría que sondear esa función cada Sleep(_n_) hasta que detecte una ventana en primer plano, incluso definiendo un HWNDPrev

tal vez te preguntes por que hay dos ejecutables y dos DLLs?
> Debido a que los ganchos se ejecutan en el contexto de una aplicación, deben coincidir con el "bitness" de la aplicación. Si una aplicación de 32 bits instala un enlace global en Windows de 64 bits, el enlace de 32 bits se inyecta en cada proceso de 32 bits (se aplican los límites de seguridad habituales). En un proceso de 64 bits, los subprocesos todavía están marcados como "enganchados". Sin embargo, debido a que una aplicación de 32 bits debe ejecutar el código de enlace, el sistema ejecuta el enlace en el contexto de la aplicación de enlace; específicamente, en el hilo que llamó SetWindowsHookEx . Esto significa que la aplicación de enlace debe continuar enviando mensajes o podría bloquear el funcionamiento normal de los procesos de 64 bits.

para poder inyectar correctamente el ShellProc, este requiere de dos ejecutables que llamen a los enlaces para las arquitecturas (32 y 64 bits) de diferentes programas, esto generaria un descanso a la CPU ya que solo intercepta la creación de nuevas ventanas (primer plano), obtiene el HWND y aplica la política...

## en que se diferencia de basicthemer5?

* 13MB aproximadamente de total bytes (basicthemer5) frente a 2.5MB (entre los dos ejecutables de dwmbsc)
* no hay sondeo de funciones ni eventos de windows, solo hay un while en cada proceso para evitar que el ShellProc bloquee su sistema
* cada ejecutable depende de cada uno de los DLLs para que pueda inyectar correctamente el HOOKPROC a cada proceso dependiendo del contexto de su arquitectura
* añadi un hootkey a dwmbsc32.exe (CTRL + SHIFT + F1) esto restaurara las politicas aplicadas a cada atributo de dwm

## INFO

* Alternativamente puede usar [dwm-bs](dwm-bs\dwm-bs.cpp), que es una versión casi parecida a BasicThemer5
esta versión es mucho más portable que usar dwmbsc32 pero a cambio de un sondeo pequeño, relativamente sigue funcionando mejor que BasicThemer5 y consume mucho menos recursos

* Sondea un WinEventHook que detecta la creación de un objeto, si este es una ventana y es diferente de la previa (HWNDPrev) aplicará la política

![image](https://github.com/LuSlower/dwmbsc/assets/148411728/34af9b3d-1343-4a19-8e10-915c939f2c87)


* si no desea usar el SysMenu de Chrome / Edge, puede usar este [tema](https://chromewebstore.google.com/detail/windows-vista-basic-theme/bkohfcingfpclphbaglfbbjbfajcepad) 
para activar el tema heredado en Chrome / Edge, necesita inicializar el programa con **-disable-windows10-custom-titlebar** para eso mejor cree un acceso directo

## COMO USAR?

* abre dwmbsc32.exe, este a su vez abrira el otro ejecutable (dwmbsc64.exe) como un proceso hijo, cuando presiones el hootkey automaticamente se cerrara junto a su proceso padre

## DERECHOS DE AUTOR Y LICENCIA
[LICENCIA](LICENSE)
