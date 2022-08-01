cmake_minimum_required(VERSION 3.21)

if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-function -ansi)
endif ()

project(nxtetris)

file(GLOB_RECURSE SOURCES "src/*.h" "src/*.c")
add_executable(nxtetris ${SOURCES} src/game/ui/end_screen.h src/game/ui/end_screen.c)

target_include_directories(nxtetris PUBLIC "src")

find_package(Curses REQUIRED)
target_link_libraries(nxtetris curses)
target_include_directories(nxtetris PRIVATE ${CURSES_INCLUDE_DIRS})
