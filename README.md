# dwm-basic
The lightest and most complete basic themer

[![Total Downloads](https://img.shields.io/github/downloads/LuSlower/dwmbsc/total.svg)](https://github.com/LuSlower/dwmbsc/releases)

![image](https://github.com/LuSlower/dwm-basic/assets/148411728/d6fdea55-0a61-4ce5-94ac-743483fe5e80)

## INFO

* If you want to establish exclusions, by default a REG_MULTI_SZ will be created in `HKCU\Software\dwm-bs` called `ExclusionList`, there you must specify which processes you do not want to be affected by the deactivation of attributes

* registers a WinEventHook that detects the EVENT_SYSTEM_FOREGROUND event that occurs when a window changes and becomes the active or foreground window, if this is different from the previous one (HWNDPrev) and is not in the exclusion list, the policy will be applied

* applies the following attributes:
> DWMWA_NCRENDERING_POLICY

> DWMWA_TRANSITIONS_FORCEDISABLED

> DWMWA_FORCE_ICONIC_REPRESENTATION (leave Peek inactive)

* if you don't want to use the SysMenu of your Chromium-based browser add it to `ExclusionList` and/or use this [tema](https://chromewebstore.google.com/detail/windows-vista-basic-theme/bkohfcingfpclphbaglfbbjbfajcepad) you need to initialize the program with **-disable-windows10-custom-titlebar** for that better create a shortcut

* You can specify the -m argument if you want to call DwmEnableMMCSS, the function will be called every time a window is in the foreground

* CTRL + SHIFT + F1 to restore everything to normal

# REF
[1](https://github.com/spddl/disableDWMactivitys)

