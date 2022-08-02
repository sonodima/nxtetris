<h1 align="center">NXTetris 🕹️</h1>

<div align="center">
  <img src="https://badgen.net/badge/lang/ANSI-C/grey"/>
  <a href="https://sonodima.github.io/nxtetris/" target="_blank"><img src="https://badgen.net/badge/docs/Doxygen/red"/></a>
  <br>
  <br>
</div>

> Cross-platform, over-engineered XTetris in C.

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

- [ncurses](https://invisible-island.net/ncurses/announce.html)
