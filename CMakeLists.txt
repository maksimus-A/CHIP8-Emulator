cmake_minimum_required(VERSION 3.16)

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
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
        set(CHIP8_SDL_TARGET PkgConfig::SDL2)
    endif()
endif()

add_executable(chip8_emulator
    main.c
    display.c
)

target_link_libraries(chip8_emulator PRIVATE ${CHIP8_SDL_TARGET})

if(MSVC)
    target_compile_options(chip8_emulator PRIVATE /W4)
else()
    target_compile_options(chip8_emulator PRIVATE -Wall -Wextra -Wpedantic)
endif()
