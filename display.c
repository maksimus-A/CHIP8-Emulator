#include<stdio.h>
#include <stdlib.h>
#include"SDL.h"
#include"display.h"
#include<string.h>


display *display_init(unsigned int width, unsigned int height) {
    display *d = malloc(sizeof(display));

    d->width = width, d->height = height;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window;
    SDL_Surface* screenSurface;

    window = SDL_CreateWindow("Chip8 Emulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        (width * PIXEL_SIZE), (height * PIXEL_SIZE), SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr, "Window could not be created: %s\n", SDL_GetError());
        return 1;
    }

    screenSurface = SDL_GetWindowSurface(window);
    

    if (!screenSurface) {
        fprintf(stderr, "Screen surface could not be created: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    d->screen = screenSurface;


    return d;
}

void display_free(display* d) {
    free(d);
    SDL_Quit();
}

void display_draw(display* d, uint8_t* buffer) {
    for (uint8_t Y = 0; Y < d->height; Y++) {
        for (uint8_t X = 0; X < d->width; X++) {
            if ((buffer[X + Y * d->width] & 0x01) == 0x01)
                SDL_SetRenderDrawColor(d->screen, 0xFF, 0xFF, 0xFF, 0xFF); // window, R, G, B, A
            else
                SDL_SetRenderDrawColor(d->screen, 0x00, 0x00, 0x00, 0xFF); // window, R, G, B, A
            SDL_Rect rectangle;

            rectangle.x = X * PIXEL_SIZE;
            rectangle.y = X * PIXEL_SIZE;
            rectangle.w = rectangle.x + 1;
            rectangle.h = rectangle.y + 1;
            SDL_RenderFillRect(d->screen, &rectangle);

        }
    }
    SDL_RenderPresent(d->screen);
}


