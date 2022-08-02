<h1 align="center">NXTetris 🕹️</h1>

<div align="center">
  <img src="https://badgen.net/badge/lang/ANSI-C/grey"/>
  <a href="https://sonodima.github.io/nxtetris/"><img src="https://badgen.net/badge/docs/Doxygen/red"/></a>
  <br>
  <br>
</div>

> Cross-platform, over-engineered XTetris written in C

## Controls

### Menu(s)

* `KBD_RIGHT` - Confirm
* `KBD_LEFT` - Exit / Back
* `KBD_UP`, `KBD_DOWN` - Menu navigation

### Game

* `KBD_LEFT`, `KBD_RIGHT` - Rotate the tetromino
* `KBD_UP`, `KBD_DOWN` - Change tetromino
* `MOUSE_X` - Position the tetromino in the x-axis
* `MOUSE_LEFT` - Drop the current tetromino in the board

## Build

<b>Before proceeding, make sure you have cmake and make installed!</b>
<br>
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

## Libraries

- [ncurses:](https://invisible-island.net/ncurses) terminal control library, enabling the construction of text user interface (TUI) applications.
