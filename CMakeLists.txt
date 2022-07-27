cmake_minimum_required(VERSION 3.21)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-function)
endif()

project(nxtetris)
add_executable(nxtetris
        src/main.c
        src/engine/media/graphics.c
        src/engine/types/point.c
        src/engine/core/vector.c
        src/engine/core/matrix.c
        src/engine/controls.c
        src/engine/utils.c src/game/game.c src/game/game.h src/game/tetromino.h src/game/tetromino.c src/game/board.h src/game/board.c)

find_package(Curses REQUIRED)
target_link_libraries(nxtetris curses)
target_include_directories(nxtetris PRIVATE ${CURSES_INCLUDE_DIRS})
