# dwmbsc
[![Total Downloads](https://img.shields.io/github/downloads/LuSlower/dwmbsc/total.svg)](https://github.com/LuSlower/dwmbsc/releases) [![PayPal Badge](https://img.shields.io/badge/PayPal-003087?logo=paypal&logoColor=fff&style=flat)](https://paypal.me/eldontweaks) 

el basic themer más ligero y completo

![image](https://github.com/LuSlower/dwm-basic/assets/148411728/d6fdea55-0a61-4ce5-94ac-743483fe5e80)

## INFO

* si desea establecer exclusiones, por defecto se creará un REG_MULTI_SZ en `HKCU\Software\dwm-bs` llamado `ExclusionList`, allí deben específicar que procesos no quieren que se vean afectados por la desactivación de atributos

* registra un WinEventHook que detecta el evento EVENT_SYSTEM_FOREGROUND que se produce cuando una ventana cambia y se convierte en la ventana activa o en primer plano, si este es diferente de la previa (HWNDPrev) y no esta en la lista de exclusión aplicará la política

* aplica los siguientes atributos:
> DWMWA_NCRENDERING_POLICY
> DWMWA_TRANSITIONS_FORCEDISABLED

tambien deshabilita DWM Blur Behind...

* si no desea usar el SysMenu de Chrome / Edge, puede usar este [tema](https://chromewebstore.google.com/detail/windows-vista-basic-theme/bkohfcingfpclphbaglfbbjbfajcepad) 
para activar el tema heredado en Chrome / Edge, necesita inicializar el programa con **-disable-windows10-custom-titlebar** para eso mejor cree un acceso directo

* posee un hotkey CTRL + SHIFT + F1 para restaurar todo a la normalidad

## DERECHOS DE AUTOR Y LICENCIA
[LICENCIA](LICENSE)
