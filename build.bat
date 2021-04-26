@echo off

set includes=/I dependencies\Glad\include /I dependencies\GLFW\include /I src /I dependencies\stb\include
set libs=dependencies\GLFW\lib\glfw3.lib dependencies\Glad\lib\glad.lib Shell32.lib User32.lib Gdi32.lib OpenGL32.lib msvcrt.lib

rem Libraries
cl /Ox /EHsc /c dependencies\stb\src\*.cpp  /I dependencies\stb\include

rem Source
cl /MT /Ox /EHsc /c src/universal/vecs/*.cpp
cl /MT /Ox /EHsc /c src/universal/mats/*.cpp
cl /MT /Ox /EHsc /c src/universal/*.cpp

cl /MT /Ox /EHsc /c src/engine/*.cpp %includes%
cl /MT /Ox /EHsc /c src/platform/*.cpp %includes%

cl /MT /Ox /EHsc /c src/game/*.cpp %includes%

cl /MT /Ox /EHsc /c src/main.cpp %includes%

rem Resources
rc resources.rc

link *.obj *.res %libs% /OUT:ttt.exe /NODEFAULTLIB:LIBCMT /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup

rem Delete intermediate files
del *.obj
del *.res