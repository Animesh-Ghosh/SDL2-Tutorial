# SDL2-Tutorial

Followed [Twinklebear's](https://github.com/Twinklebear) [SDL2 Tutorial](https://www.willusher.io/pages/sdl2/).

## Building

Used the MinGW toolchain. Makefile has not been changed much when compared to the tutorial. To build each lesson's output, simply run `make` with `SRC=` the source file that has to be compiled.

An example:
```PowerShell
mingw32-make SRC=lesson1.cpp
```

*Note: The directory structure is as follows:*

```
/
- bin/
- include/
- lib/
-
...
*other resources and source files*
```