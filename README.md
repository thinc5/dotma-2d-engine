# Dotma's Visual Novel Engine

## Compilation

### Windows
- Requires mingw32, gcc, make, sdl2(mingw32), sdl_image(mingw32), sdl_ttf(mingw32) and sdl_mixer(mingw32)

### Linux
- Install sdl2-dev, sdl2-image, sdl2-ttf and sdl2-mixer from your package manager

### macOS
- Install sdl2, sdl2_image, sdl2_ttf and sdl2_mixer using the Homebrew package manager

Then run - `make` to build the game normally, - `make -j` for compiling on multiple cores and finally `make debug` to enable the `-g` flag and include the debug features in the executable.

## Running
Currently only runs when executed from the executables root directory. This will be changed later but it currently relies on it's working environment being its directory so executing it like `./stuff/output.exe` would prevent it from finding its assets. For now just ensure you're in it's directory before executing it. (Clicking on the .exe or .out works too!)

## About
Simple engine for rendering 2d texutures with simple sound playback.
This project was started to help a friend make his dream game (a visual novel) however eventually the base of this engine may be added to, in order to support more advanced functions and mechanics.

## Disclaimer
This engine is very silly and not as optimised or modular/reusable as I would like.
That is becasue this is a hobby project and I dont always have time to make things as perfect as I would like.
Working on it has been lots of fun, so feel free to make a pull request or post an issue and/or feature request!

## Licence
This repo is licenced under GPL-3
