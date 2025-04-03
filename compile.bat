cd src
clang -target x86_64-windows -DUTILS_EXPORTS -shared utils.c -o standard.dll -Wl,/DLL,/IMPLIB:standard.lib -I.
del standard.exp
cd ..
if not exist build mkdir build
if not exist include mkdir include
move src\standard.dll build\standard.dll
move src\standard.lib build\standard.lib
copy src\utils.h include\
pause
