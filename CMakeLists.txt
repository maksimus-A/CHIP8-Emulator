cmake_minimum_required(VERSION 3.16)

# vcpkg: toolchain must be set before project() so find_package(SDL2 CONFIG) sees installed ports.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(_vcpkg_toolchain "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    if(EXISTS "${_vcpkg_toolchain}")
        set(CMAKE_TOOLCHAIN_FILE "${_vcpkg_toolchain}" CACHE STRING "")
    endif()
endif()

project(CHIP8Emulator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(SDL2 CONFIG QUIET)
if(SDL2_FOUND)
    set(CHIP8_SDL_TARGET SDL2::SDL2)
else()
    find_package(SDL2 QUIET)
    if(SDL2_FOUND)
        set(CHIP8_SDL_TARGET SDL2::SDL2)
    else()
        if(WIN32)
            message(FATAL_ERROR
                "SDL2 not found. Install it (e.g. vcpkg: vcpkg install sdl2:x64-windows) and configure with "
                "-DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake")
        endif()
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
        set(CHIP8_SDL_TARGET PkgConfig::SDL2)
    endif()
endif()

add_executable(chip8_emulator
    main.c
    display.c
)

if(WIN32 AND TARGET SDL2::SDL2main)
    # SDL.h maps main -> SDL_main; SDL2main supplies the real entry point for MSVC.
    target_link_libraries(chip8_emulator PRIVATE SDL2::SDL2main ${CHIP8_SDL_TARGET})
else()
    target_link_libraries(chip8_emulator PRIVATE ${CHIP8_SDL_TARGET})
endif()

if(MSVC)
    target_compile_options(chip8_emulator PRIVATE /W4)
else()
    target_compile_options(chip8_emulator PRIVATE -Wall -Wextra -Wpedantic)
endif()
