#include<stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include"SDL.h"
#include"display.h"
#include"chip8.h"

Running = true;
// OLD CHIP8-EMULATOR.C BC IM STUPID
uint8_t program_load(char* filename) {
    // Open file
    FILE* pFile;
    if (fopen_s(&pFile, filename, "rb") == NULL)
    {
        fputs("File error", stderr);
        return false;
    }

    // Check file size
    fseek(pFile, 0, SEEK_END);
    long lSize = ftell(pFile);
    rewind(pFile);
    printf("Filesize: %d\n", (int)lSize);

    // Allocate memory to contain the whole file
    char* buffer = (char*)malloc(sizeof(char) * lSize);
    if (buffer == NULL)
    {
        fputs("Memory error", stderr);
        return false;
    }

    // Copy the file into the buffer
    size_t result = fread(buffer, 1, lSize, pFile);
    if (result != lSize)
    {
        fputs("Reading error", stderr);
        return false;
    }

    // Copy buffer to Chip8 memory
    if ((4096 - 512) > lSize)
    {
        for (int i = 0; i < lSize; ++i)
            memory[i + 512] = buffer[i];
    }
    else
        printf("Error: ROM too big for memory");

    // Close file, free buffer
    fclose(pFile);
    free(buffer);

    return 0;
}
void initialize()
{
    // Initialize registers and memory once
    pc = 0x200;		// Program counter starts at 0x200 (Start adress program)
    opcode = 0;			// Reset current opcode
    I = 0;			// Reset index register
    sp = 0;			// Reset stack pointer

    // Clear display
    for (int i = 0; i < 2048; ++i)
        gfx[i] = 0;

    // Clear stack
    for (int i = 0; i < 16; ++i)
        stack[i] = 0;

    for (int i = 0; i < 16; ++i)
        key[i] = V[i] = 0;

    // Clear memory
    for (int i = 0; i < 4096; ++i)
        memory[i] = 0;

    // Load fontset
    for (int i = 0; i < 80; ++i)
        memory[i] = chip8_fontset[i];

    // Reset timers
    delay_timer = 0;
    sound_timer = 0;

    // Clear screen once
    drawFlag = true;
}

void emulateCycle()
{
    // Fetch Opcode
    opcode = memory[pc + 0];
    opcode <<= 8;
    opcode |= memory[pc + 1];
    // Decode opcode
    // opcode & 0xF000 is bit AND operation, will only return first 4 digits of number
    switch (opcode & 0xF000) // Basic disassembler
    {
    case 0x0000:
        switch (opcode & 0x000F)
        {
        case 0x0000: // 00E0: Clear the screen
            for (uint16_t i = 0; i < WIDTH * HEIGHT; i++) {
                gfx[i] = 0;
            }
            pc += 2;
            break;
        case 0x000E: // 00EE: return from subroutine
            // IDK LOL
            break;

        default:
            printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
        }
        break;
    case 0x2000: // 2NNN: Calls subroutine at address NNN
        stack[sp] = pc; // set stack pointer to program counter
        ++sp; // increase stack pointer
        pc = opcode & 0x0FFF;
        break;
        // CHECK
    case 0x3000: // 3XNN: Skips next instruction if VX equals NN
        if (V[opcode & 0x0F00 >> 8] == (opcode & 0x00FF)) {
            pc += 4;
        }
        else { pc += 2; }
        break;
    case 0x4000: //4xnn: Skips instruction if VX DOESN'T equal NN
        if (V[opcode & 0x0F00 >> 8] != (opcode & 0x00FF)) {
            pc += 4;
        }
        else { pc += 2; }
        break;
    case 0x5000: // 5XY0: skips instruction if VX == VY
        if (V[opcode & 0x0F00 >> 8] == V[opcode & 0x00F0 >> 4]) {
            pc += 4;
        }
        else { pc += 2; }
        break;
    case 0x6000: // 6XNN: Sets VX to NN.
        V[opcode & 0x0F00 >> 8] = (opcode & 0x00FF);
        pc += 2;
        break;
    case 0x7000: //7XNN: Adds NN to VX.
        V[opcode & 0x0F00 >> 8] += (opcode & 0x00FF);
        pc += 2;
        break;
    case 0x8000:
        switch (opcode & 0x000F) { // opcode & 0x000F makes the number only store the last 4 bits (as 000F is 16 so only 16 numbers total) (and & is bit AND)
        case 0x0000: // 8XY0: Sets VX to VY.
            V[opcode & 0x0F00 >> 8] = V[opcode & 0x00F0 >> 4];
            break;
        case 0x0001: // 8XY1: Sets VX to VX | VY
            V[opcode & 0x0F00 >> 8] |= V[opcode & 0x00F0 >> 4];
            break;
        case 0x0002: // 8XY2: Sets VX to VX & VY
            V[opcode & 0x0F00 >> 8] &= V[opcode & 0x00F0 >> 4];
            break;
        case 0x0003: // 8XY3: Sets VX to VY.
            V[opcode & 0x0F00 >> 8] ^= V[opcode & 0x00F0 >> 4];
            break;
        case 0x0004: // 8XY4: Adds VY to VX, VF is 1 if there is carry, else 0
        // V[opcode & 0x00F0 >> 4] retrives VY from the opcode (0x00Y0)(same for VX but bit shift is 8 bc thats where it is in the address (0x0X00))
        // Somehow this if statement checks if the sum goes over 255
        // I KNOW HOW
        // It checks if X is greater than 255-Y, because if it is the sum is greater than 255

        // x is 8 and y is 4

            if (V[opcode & 0x00F0 >> 4] > (0xFF - V[opcode & 0x0F00 >> 8])) { // Y > 255-X
                V[0xF] = 1; // carry flag is 1
            }
            else {
                V[0xF] = 0;
            }
            V[opcode & 0x0F00 >> 8] += V[opcode & 0x00F0 >> 4]; // Add y to x
            pc += 2; // increment program counter
            break;
        case 0x0005: // 8XY5: VY is subtracted from VX
            if (V[opcode & 0x00F0 >> 4] < (0xFF - V[opcode & 0x0F00 >> 8])) {// If y < x
                V[0xF] = 1; // carry flag is 1
            }
            else {
                V[0xF] = 0;
            }
            V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4]; // Subtract Y from X
            pc += 2; // increment program counter
            break;
        case 0x0006: // 8XY6: Stores the least significant bit of VX in VF and then shifts VX to the right by 1
            V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
            V[(opcode & 0x0F00) >> 8] >>= 1;
            pc += 2;
            break;
        case 0x0007: // 8XY7: Sets VX to VY - VX. VF is 0 on borrow, 1 if not
            if (V[opcode & 0x00F0 >> 4] < (0xFF - V[opcode & 0x0F00 >> 8])) {// If y < x
                V[0xF] = 1; // carry flag is 1
            }
            else {
                V[0xF] = 0;
            }
            V[(opcode & 0x0F00) >> 8] = V[opcode & 0x00F0 >> 4] - V[opcode & 0x0F00 >> 8]; // Subtract Y from X
            pc += 2; // increment program counter
            break;
        case 0x000E: // 8XYE: Stores the most significant bit of VX in VF and then shifts VX to the left by 1
            V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
            V[(opcode & 0x0F00) >> 8] <<= 1;
            pc += 2;
            break;
        }
    case 0x9000: // 9XY0: Skips the next instruction if VX doesn't equal VY.
        if (V[opcode & 0x0F00 >> 8] != V[opcode & 0x00F0 >> 4]) {
            pc += 4;
        }
        else { pc += 2; }
        break;
    case 0xA000: // ANNN: Sets I to the address NNN
      // Execute opcode
        I = opcode & 0x0FFF;
        pc += 2;
        break;
    case 0xB000: // BNNN: jUMPS TO ADDRESS nnn PLUS V0.
        pc = (opcode & 0x0FFF) + V[0];
        break;
    case 0xC000: {// CXNN: Sets VX to the result of a bitwise and operation on a random number (0-255) and NN.
        V[opcode & 0x0F00 >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
        pc += 2;
    }
               break;
    case 0xD000:
    {
        unsigned short x = V[(opcode & 0x0F00) >> 8];
        unsigned short y = V[(opcode & 0x00F0) >> 4];
        unsigned short height = opcode & 0x000F;
        unsigned short pixel;

        V[0xF] = 0;
        for (int yline = 0; yline < height; yline++)
        {
            pixel = memory[I + yline];
            for (int xline = 0; xline < 8; xline++)
            {
                if ((pixel & (0x80 >> xline)) != 0)
                {
                    if (gfx[(x + xline + ((y + yline) * 64))] == 1)
                        V[0xF] = 1;
                    gfx[x + xline + ((y + yline) * 64)] ^= 1;
                }
            }
        }

        drawFlag = true;
        pc += 2;
    }
    break;
    case 0xE000:
        switch (opcode & 0x000F)
        {
            // EX9E: Skips the next instruction
            // if the key stored in VX is pressed
        case 0x000E: {
            if (key[V[(opcode & 0x0F00) >> 8]] != 0)
                pc += 4;
            else
                pc += 2;

        }
                   break;
        case 0x0001: { //EXA1: Skips the next instruction if the key stored in VX isn't pressed.
            if (key[V[(opcode & 0x0F00) >> 8]] == 0)
                pc += 4;
            else
                pc += 2;
        }
                   break;
        }
    case 0xF000: {
        switch (opcode & 0x00FF) {
        case 0x0007: { // fx07: sets VX to value of delay timer
            V[(opcode & 0x0F00) >> 8] = delay_timer;
            pc += 2;
        }
                   break;
        case 0x000A: { // fx0A: a key press is awaited then stored in VX
            V[(opcode & 0x0F00) >> 8] = delay_timer;
            pc += 2;
        }
                   break;
        case 0x0015: { // fx15: sets delay timer to value of VX
            delay_timer = V[(opcode & 0x0F00) >> 8];
        }
                   break;
        case 0x0018: { // fx18: sets sound timer to value of VX
            sound_timer = V[(opcode & 0x0F00) >> 8];
        }
                   break;
        case 0x001E: { // FX1E: Adds VX to I. Vf not affected.
            I += V[(opcode & 0x0F00) >> 8];
        }
                   break;
        case 0x0029: { // FX29: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.

        }
                   break;
        case 0x0033: {
            // FX33: Stores the binary-coded decimal representation of VX, with the most significant of three digits
            // at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2.
            memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
            memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
            memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
            pc += 2;
        }
                   break;
        case 0x0055: { // FX55: Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, I unmodified
            for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
                memory[I + i] = V[i];
            }
            pc += 2;
        }
                   break;
        case 0x0065: {
            for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
                V[i] = memory[I + i];
            }
            pc += 2;
        }
                   break;
        }
    }
               break;
    default:
        printf("Unknown opcode: 0x%X\n", opcode);
    }
    // Execute Opcode

      // Update timers
    if (delay_timer > 0)
        --delay_timer;

    if (sound_timer > 0)
    {
        if (sound_timer == 1)
            printf("BEEP!\n");
        --sound_timer;
    }
}

int main(int argc, char** argv) {
    /* would have been smart to do this OH WELL!
#define OPCODE_X ((cpu.opcode & 0x0F00) >> 8)
#define OPCODE_Y ((cpu.opcode & 0x00F0) >> 4)
#define OPCODE_N (cpu.opcode & 0x000F)
#define OPCODE_KK (cpu.opcode & 0x00FF)
#define OPCODE_NNN (cpu.opcode & 0x0FFF)*/


// Set up render system and register input callbacks
//setupGraphics();
//setupInput();

    // Initialize the Chip8 system and load the game into the memory

    display* d = display_init(WIDTH, HEIGHT);
    program_load("tetris.c8");

    // Emulation loop
    while (Running) {
        // Emulate one cycle
        emulateCycle();

        // If the draw flag is set, update the screen
        if (drawFlag) {
            display_draw(d, gfx);
            drawFlag = false;
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                Running = false;
                break;
            case SDL_KEYUP:
            case SDL_KEYDOWN:
                for (uint8_t i = 0; i < NUM_KEYS; i++) {
                    if (event.key.keysym.sym == scancodes[i]) { // if key pressed is in the array
                        uint8_t state = (event.type == SDL_KEYDOWN) ? 1 : 0;
                        key_set(key_map[i], state);
                        break;
                    }
                }
                break;
            default:
                break;
            }

            SDL_Delay(1); // probably slows down clock of CPU?
        }
        display_free(d); // frees memory allocated to display
        // no chip8 memory so dont have to free that
    }
    return 0;
}
