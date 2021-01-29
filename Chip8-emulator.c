#include<stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include"SDL.h"
#include"display.h"





void keyboardDown(unsigned char Key, int x, int y)
{
    if (Key == 27)    // esc
        exit(0);
        
    if (Key == '1')		    key[0x1] = 1;
    else if (Key == '2')	key[0x2] = 1;
    else if (Key == '3')	key[0x3] = 1;
    else if (Key == '4')	key[0xC] = 1;

    else if (Key == 'q')	key[0x4] = 1;
    else if (Key == 'w')	key[0x5] = 1;
    else if (Key == 'e')	key[0x6] = 1;
    else if (Key == 'r')	key[0xD] = 1;

    else if (Key == 'a')	key[0x7] = 1;
    else if (Key == 's')	key[0x8] = 1;
    else if (Key == 'd')	key[0x9] = 1;
    else if (Key == 'f')	key[0xE] = 1;

    else if (Key == 'z')	key[0xA] = 1;
    else if (Key == 'x')	key[0x0] = 1;
    else if (Key == 'c')	key[0xB] = 1;
    else if (Key == 'v')	key[0xF] = 1;

    //printf("Press key %c\n", key);
}

void keyboardUp(unsigned char Key, int x, int y)
{
    if (Key == '1')		key[0x1] = 0;
    else if (Key == '2')	key[0x2] = 0;
    else if (Key == '3')	key[0x3] = 0;
    else if (Key == '4')	key[0xC] = 0;

    else if (Key == 'q')	key[0x4] = 0;
    else if (Key == 'w')	key[0x5] = 0;
    else if (Key == 'e')	key[0x6] = 0;
    else if (Key == 'r')	key[0xD] = 0;

    else if (Key == 'a')	key[0x7] = 0;
    else if (Key == 's')	key[0x8] = 0;
    else if (Key == 'd')	key[0x9] = 0;
    else if (Key == 'f')	key[0xE] = 0;

    else if (Key == 'z')	key[0xA] = 0;
    else if (Key == 'x')	key[0x0] = 0;
    else if (Key == 'c')	key[0xB] = 0;
    else if (Key == 'v')	key[0xF] = 0;
}
