@echo off
set SFML_ROOT=D:\SFML\SFML-3.1.0
call "D:\visual studio placement\VC\Auxiliary\Build\vcvars64.bat"
cl /nologo /std:c++17 /utf-8 /EHsc /MD /DSFML_STATIC ^
   /I . /I source /I source\common /I source\crud /I source\database /I source\gui /I source\render ^
   /I"%SFML_ROOT%\include" ^
   main.cpp /Fe:app.exe ^
   /link /LIBPATH:"%SFML_ROOT%\lib" sfml-graphics-s.lib sfml-window-s.lib sfml-system-s.lib freetype.lib harfbuzz.lib opengl32.lib gdi32.lib winmm.lib user32.lib advapi32.lib shell32.lib
pause