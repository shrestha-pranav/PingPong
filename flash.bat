C:\OlimexODS\yagarto\bin\make main.hex
if errorlevel 1 goto END
C:\OlimexODS\openocd-0.6.1\bin-x64\openocd-x64-0.6.1 -f ../scripts/interface/olimex-jtag-tiny.cfg -f flash.cfg
:END