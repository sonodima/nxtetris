<h1 align="center">NXTetris 🕹️</h1>

<div align="center">
  <img src="https://badgen.net/badge/lang/ANSI-C/grey"/>
  <a href="https://sonodima.github.io/nxtetris/"><img src="https://badgen.net/badge/docs/Doxygen/red"/></a>
  <br>
  <br>
</div>
 
> Cross-platform, over-engineered XTetris written in C

## Features

- [x] Single-player game mode
- [x] Two-players game mode
- [x] Player vs. AI game mode _(AI algorithm is still really basic)_
- [x] Piece control with keyboard and mouse
- [x] 2D CLI graphics abstraction
- [x] Audio output support

## Controls

### Menu(s)

* <kbd>KB_RIGHT</kbd> - Confirm
* <kbd>KB_LEFT</kbd> - Exit / Back
* <kbd>KB_UP</kbd>, <kbd>KB_DOWN</kbd> - Menu navigation

### Game

* <kbd>KB_LEFT</kbd>, <kbd>KB_RIGHT</kbd> - Rotate the tetromino
* <kbd>KB_UP</kbd>, <kbd>KB_DOWN</kbd> - Change tetromino
* <kbd>MOUSE_X</kbd> - Position the tetromino in the x-axis
* <kbd>MOUSE_LEFT</kbd> - Drop the current tetromino in the board

## Build

<b>Requirements:</b>

- cmake
- make
- pkg-config _(macOS/Linux only)_

---

Building the program is done by running the following commands:

```bash
git clone --recursive https://github.com/sonodima/nxtetris
cd nxtetris

mkdir -p build/release
cd build/release

cmake ../.. -DCMAKE_BUILD_TYPE=Release
make
```

The output executable will be named `nxtetris` and will be placed in `build/release`.

<b>NOTE:</b> The binary needs to be shipped with the `resources` folder _(which is copied to the build directory at
compile time)_ in order to work properly. _(without it the audio won't play)_

### Windows Compilation

Due to ncurses limitations, on Windows MinGW is required to compile the program.<br>
You can use the following command to generate the Makefile for MinGW:

```bash
cmake ../.. -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles"
```

## Libraries

- [ncurses:](https://invisible-island.net/ncurses) terminal control library, enabling the construction of text user
  interface (TUI) applications.
- [portaudio:](http://www.portaudio.com/) cross-platform audio I/O library.
- [libsndfile:](http://www.mega-nerd.com/libsndfile/) audio file I/O library.

All the libraries are managed by [vcpkg](https://vcpkg.io/) and are automatically obtained during the build process.

<b>NOTE:</b> The first build will take a while since all the libraries need to be downloaded and built.
