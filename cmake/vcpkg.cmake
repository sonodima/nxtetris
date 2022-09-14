cmake_minimum_required(VERSION 3.21)

include(FetchContent)

message(STATUS "Downloading: microsoft/vcpkg")
FetchContent_Declare(vcpkg GIT_REPOSITORY https://github.com/microsoft/vcpkg)
FetchContent_MakeAvailable(vcpkg)

set(CMAKE_TOOLCHAIN_FILE ${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake)
message(STATUS "Toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

