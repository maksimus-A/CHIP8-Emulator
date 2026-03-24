#pragma once
// CHIP8 Stuff
#ifndef CHIP8_H
#include<stdbool.h>
#include<stdint.h>
#include"SDL.h"

#define CHIP8_H

#define WIDTH         64
#define HEIGHT        32
#define NUM_KEYS      16
#define PROGRAM_START 0x200
#define MEM_SIZE      4096


unsigned short opcode; // opcodes
unsigned char memory[4096]; // memory allocation
unsigned char V[16]; // CPU registers
unsigned short I; // Index register
unsigned short pc; // Program counter (register)
uint8_t gfx[64 * 32]; // Size of screen/pixel render
unsigned char delay_timer; // Counts down to zero
unsigned char sound_timer; // Counts down to zero (buzzer sounds)
unsigned short stack[16]; // stack simulation
unsigned short sp; // stack pointer
unsigned char key[16]; // store key presses

static const uint8_t chip8_fontset[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
uint8_t scancodes[NUM_KEYS] = {
    SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R,
    SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F,
    SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V
};
uint8_t key_map[NUM_KEYS] = {
    0x01, 0x02, 0x03, 0x0c, // 1 2 3 c
    0x04, 0x05, 0x06, 0x0d, // 4 5 6 d
    0x07, 0x08, 0x09, 0x0e, // 7 8 9 e
    0x0a, 0x00, 0x0b, 0x0f  // a 0 b f
};
bool drawFlag = false;

static inline void key_set(uint8_t Key, uint8_t val) { key[Key & 0xF] = val; }
#endif
