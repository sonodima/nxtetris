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

The output executable will be named `nxtetris` and will be placed in `build/release`.<br>
The binary needs to be shipped with the `resources` folder (which is copied to the build directory at compile time) in
order to work properly. _(without it the audio won't play)_

## Libraries

- [ncurses:](https://invisible-island.net/ncurses) terminal control library, enabling the construction of text user
  interface (TUI) applications.
- [portaudio:](http://www.portaudio.com/) cross-platform audio I/O library.
- [libsndfile:](http://www.mega-nerd.com/libsndfile/) audio file I/O library.

All the libraries are managed by [vcpkg](https://vcpkg.io/) and are automatically downloaded and built during the build
process.

<b>NOTE:</b> The first build will take a while since all the libraries need to be downloaded and built.
