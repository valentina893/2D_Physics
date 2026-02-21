// input.c

#include "input.h"

input input_create() {
    return (input){0, 0, 0};
}

void input_read(input* input, int* running) {

    if (input == NULL || running == NULL) return;

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *running = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                input->mouse_left_press = 1;
                break;
            default:
                break;
        }
    }

    return;

}