#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H
// CHIP8 Stuff
#define WIDTH         64
#define HEIGHT        32
#define NUM_KEYS      16
#define PROGRAM_START 0x200
#define MEM_SIZE      4096

#include <stdint.h>

#define PIXEL_SIZE 10

struct display_t {
    SDL_Surface* screen;
    uint8_t width, height;
} display_t;

typedef struct display_t display;

display* display_init(uint8_t width, uint8_t height);
void display_free(display* d);
void display_draw(display* d, uint8_t* buffer);

#endif