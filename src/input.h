// input.h

#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>

/*
Input class that stores information about mouse input.
*/
typedef struct input {
    int mouse_left_press;
    int mouse_x, mouse_y;
} input;

/*
Initializes input class values to zero.
*/
input input_create();

/*
Called in game_run() loop to check/update user inputs.
*/
void input_read(input* input, int* running);

#endif