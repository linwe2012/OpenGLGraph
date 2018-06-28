# OpenGLGraph

# Overview

This is a library that supports most of functions in "Graphics.h" used in ZJU for teaching.

It uses OpenGL to render all graphic component. Therefore, it inevitably disagrees with "Graphic.h" which use Win API to draw.

I have tried my best to make it compatible to the original library.

You just have to include `OpenGLGraph.h`, which contains both functions in "grahpics.h" & "extgraph.h".

Most codes that works on old graphic library just need minor adjustment and they can be compiled and run.

To see demos:
- [house](https://github.com/linwe2012/OpenGLGraph/blob/master/OpenGLT3/test_house.cpp)
- [painter](https://github.com/linwe2012/OpenGLGraph/blob/master/OpenGLT3/test_painter.cpp)
- [some wird flowerlike shape](https://github.com/linwe2012/OpenGLGraph/blob/master/OpenGLT3/test_someflower.cpp)
- [text](https://github.com/linwe2012/OpenGLGraph/blob/master/OpenGLT3/test_text.cpp)

Note that Camera is default turned on, press `esc` to turn off.

If you don't wanna compile it. [download](https://github.com/linwe2012/OpenGLGraph/releases) compiled version of these demos.
Don't forget to copy `freetype.dll` to `C:\Windows\SysWOW64` or any directory your system stores 32bits `dll` files. Or [downlaod freetype](https://www.freetype.org/) compatibale to your computer.
If `VCRUNTIME140.dll` missing, please download [Visual C++ Redistributable for Visual Studio 2015](https://www.microsoft.com/en-us/download/details.aspx?id=48145)  install x86 version.


# TODO
- Things are disproportioned
- Mouse positon is inaccurate
- Text rendering is buggy
- All color must be registered before use, i will add colors to it.
- Key board Iput is buggy cuz every time it refresh will scan the keys.

# Wiki
- [Compile](https://github.com/linwe2012/CProject/wiki/How-To-Compile)
- Featured Functions Intro
- [Dynamic Object](https://github.com/linwe2012/OpenGLGraph/wiki/Dynamic-Object)(Major Data type)
