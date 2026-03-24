#include<stdio.h>
#include <stdlib.h>
#include"SDL.h"
#include"display.h"
#include<string.h>


display *display_init(uint8_t width, uint8_t height) {
    display *d = malloc(sizeof(display));
    if (!d) {
        fprintf(stderr, "Failed to allocate display struct\n");
        return NULL;
    }

    d->width = width, d->height = height;
    d->window = NULL;
    d->renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL could not initialize: %s\n", SDL_GetError());
        free(d);
        return NULL;
    }

    d->window = SDL_CreateWindow("Chip8 Emulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        (width * PIXEL_SIZE), (height * PIXEL_SIZE), SDL_WINDOW_SHOWN);

    if (d->window == NULL) {
        fprintf(stderr, "Window could not be created: %s\n", SDL_GetError());
        SDL_Quit();
        free(d);
        return NULL;
    }

    // WSL/X11 can behave badly with accelerated+vsync; try robust fallbacks.
    d->renderer = SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (d->renderer == NULL) {
        d->renderer = SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED);
    }
    if (d->renderer == NULL) {
        d->renderer = SDL_CreateRenderer(d->window, -1, SDL_RENDERER_SOFTWARE);
    }
    if (d->renderer == NULL) {
        fprintf(stderr, "Renderer could not be created: %s\n", SDL_GetError());
        SDL_DestroyWindow(d->window);
        SDL_Quit();
        free(d);
        return NULL;
    }

    if (SDL_SetRenderDrawBlendMode(d->renderer, SDL_BLENDMODE_NONE) != 0) {
        fprintf(stderr, "Warning: Failed to disable blend mode: %s\n", SDL_GetError());
    }

    return d;
}

void display_free(display* d) {
    if (!d) {
        return;
    }
    if (d->renderer) {
        SDL_DestroyRenderer(d->renderer);
    }
    if (d->window) {
        SDL_DestroyWindow(d->window);
    }
    free(d);
    SDL_Quit();
}

void display_draw(display* d, uint8_t* buffer) {
    if (!d || !d->renderer || !buffer) {
        return;
    }

    SDL_SetRenderDrawColor(d->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(d->renderer);

    SDL_Rect rectangle;
    rectangle.w = PIXEL_SIZE;
    rectangle.h = PIXEL_SIZE;

    for (uint8_t Y = 0; Y < d->height; Y++) {
        for (uint8_t X = 0; X < d->width; X++) {
            if ((buffer[X + Y * d->width] & 0x01) != 0x01) {
                continue;
            }
            SDL_SetRenderDrawColor(d->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            rectangle.x = X * PIXEL_SIZE;
            rectangle.y = Y * PIXEL_SIZE;
            SDL_RenderFillRect(d->renderer, &rectangle);

        }
    }
    SDL_RenderPresent(d->renderer);
}


