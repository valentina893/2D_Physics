// input.c

#include "input.h"

input input_create() {
    return (input){0, 0, 0, 0, 0};
}

void input_read(input* input, int* running) {

    if (input == NULL || running == NULL) return;

    input->mouse_left_pressed = 0;
    input->mouse_left_released = 0;

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *running = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                input->mouse_left_down = 1;
                input->mouse_left_pressed = 1;
                input->mouse_x = event.motion.x;
                input->mouse_y = event.motion.y;
                break;
            case SDL_MOUSEBUTTONUP:
                input->mouse_left_down = 0;
                input->mouse_left_released = 1;
                break;
            case SDL_MOUSEMOTION:
                input->mouse_x = event.motion.x;
                input->mouse_y = event.motion.y;
                break;
            default:
                break;
        }
    }

    return;

}