@echo off

set includes=/I dependencies\Glad\include /I dependencies\GLFW\include /I src /I dependencies\stb\include
set libs=dependencies\GLFW\lib\glfw3.lib dependencies\Glad\lib\glad.lib Shell32.lib User32.lib Gdi32.lib OpenGL32.lib msvcrt.lib

rem Libraries
cl /EHsc /c dependencies\stb\src\*.cpp  /I dependencies\stb\include

cl /EHsc /c src/universal/vecs/*.cpp
cl /EHsc /c src/universal/mats/*.cpp
cl /EHsc /c src/universal/*.cpp

cl /EHsc /c src/engine/*.cpp %includes%
cl /EHsc /c src/platform/*.cpp %includes%

cl /EHsc /c src/game/*.cpp %includes%

cl /EHsc /c src/main.cpp %includes%

link *.obj %libs% /OUT:game.exe /NODEFAULTLIB:LIBCMT

del *.obj