cmake_minimum_required(VERSION 3.21)

include(cmake/vcpkg.cmake)

if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-function -ansi)
endif ()

project(nxtetris)

# Libraries (downloaded by vcpkg)
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
find_package(portaudio CONFIG REQUIRED)
find_package(SndFile CONFIG REQUIRED)

# Source Files
file(GLOB_RECURSE SOURCES "src/*.h" "src/*.c")
add_executable(nxtetris ${SOURCES})

target_include_directories(nxtetris PUBLIC "src" PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(nxtetris PRIVATE curses portaudio_static SndFile::sndfile)

# Copy Resource Files
file(COPY resources/audio DESTINATION resources)
