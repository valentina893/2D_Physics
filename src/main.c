// main.c

#include "input.h"
#include "renderer.h"
#include "rgbd2.h"

#define window_width 640
#define window_height 480

int main() {

    renderer _renderer;
    if (renderer_create(&_renderer, "physics", window_width, window_height) == 0) {
        printf("error making renderer\n");
        return 0;
    }

    input _input = input_create();

    rgbd2* rgbd_array = (rgbd2*)malloc(sizeof(rgbd2) * 10);
    int arr_size = 0;

    int running = 1;

    while (running) {
        input_read(&_input, &running);

        int x, y;
        int rgbd_held = -1;

        if (_input.mouse_left_press) {
            SDL_GetMouseState(&x, &y);
            for (int i = 0; i < arr_size; i++) {
                if (x >= rgbd_array[i].pos.x-rgbd_array[i].width/2 && x <= rgbd_array[i].pos.x+rgbd_array[i].width/2 && y >= rgbd_array[i].pos.y-rgbd_array[i].height/2 && y <= rgbd_array[i].pos.x+rgbd_array[i].height/2) {
                    rgbd_held = i;
                    break;
                }
            }
            // spawn an object
            if (rgbd_held == -1 && arr_size < 10) {
                rgbd_array[arr_size] = rgbd2_create(x-(50/2), y-(50/2), 100, 0, 50, 50);
                arr_size++;
                _input.mouse_left_press = 0;
            }
        }

        for (int i = 0; i < arr_size; i++) {
            if (i == rgbd_held) {
                rgbd_array[i].pos.x = x;
                rgbd_array[i].pos.y = y;
            } else {
                rgbd2_applyForce(&rgbd_array[i], 0, 980.0f * rgbd_array[i].mass);
                rgbd2_integrateEuler(&rgbd_array[i], 1.0f / 60.0f);
                rgbd2_windowCollision(&rgbd_array[i], window_width, window_height);
            }
        }

        for (int i = 0; i < arr_size-1; i++) {
            for (int j = 1; j < arr_size; j++) {
                rgbd2_objectCollision(&rgbd_array[i], &rgbd_array[j]);
            }
        }

        renderer_clear(&_renderer, 0, 0, 0, 0);
        for (int i = 0; i < arr_size; i++) {
            renderer_drawRect(&_renderer, rgbd_array[i].pos.x, rgbd_array[i].pos.y, rgbd_array[i].width, rgbd_array[i].height, 100, 100, 100, 0);
        }
        renderer_present(&_renderer);
    }

    renderer_delete(&_renderer);
    return 0;

}