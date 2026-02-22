// main.c

#include "input.h"
#include "renderer.h"
#include "rgbd2.h"

#define window_width 640
#define window_height 480

void update_rgbd(rgbd2* rgbd_array, int arr_size, int pos_held) {
    for (int i = 0; i < arr_size; i++) {
        if (i != pos_held) {
            rgbd2_applyForce(&rgbd_array[i], 0, 980.0f * rgbd_array[i].mass);
            rgbd2_integrateEuler(&rgbd_array[i], 1.0f / 60.0f);
            rgbd2_windowCollision(&rgbd_array[i], window_width+(50/2), window_height+(50/2));
        }
    }

    for (int i = 0; i < arr_size-1; i++) {
        for (int j = 1; j < arr_size; j++) {
            rgbd2_objectCollision(&rgbd_array[i], &rgbd_array[j]);
        }
    }
}

void draw(renderer* _renderer, rgbd2* rgbd_array, int arr_size) {
    renderer_clear(_renderer, 0, 0, 0, 0);
    for (int i = 0; i < arr_size; i++) {
        renderer_drawRect(_renderer, rgbd_array[i].pos.x-(50/2), rgbd_array[i].pos.y-(50/2), rgbd_array[i].width, rgbd_array[i].height, 100, 100, 100, 0);
    }
    renderer_present(_renderer);
}

int main() {

    renderer _renderer;
    if (renderer_create(&_renderer, "physics", window_width, window_height) == 0) {
        printf("error making renderer\n");
        return 0;
    }

    input _input = input_create();

    rgbd2* rgbd_array = (rgbd2*)malloc(sizeof(rgbd2) * 10);
    int arr_size = 0;
    //rgbd_array[0] = rgbd2_create(window_width/2, window_height/2, 100, 0, 50, 50);
    //arr_size++;

    int running = 1;

    int pos_held = -1;

    while (running) {

        input_read(&_input, &running);

        if (_input.mouse_left_pressed) {

            int clicked_idx = -1;

            // check if object was clicked
            for (int i = 0; i < arr_size; i++) {
                if (_input.mouse_x >= rgbd_array[i].pos.x-rgbd_array[i].width/2
                && _input.mouse_x <= rgbd_array[i].pos.x+rgbd_array[i].width/2
                && _input.mouse_y >= rgbd_array[i].pos.y-rgbd_array[i].height/2
                && _input.mouse_y <= rgbd_array[i].pos.y+rgbd_array[i].height/2) {
                    clicked_idx = i;
                    //pos_held = i;
                    break;
                }   
            }

            if (clicked_idx != -1) {
                // dragging object
                pos_held = clicked_idx;
            } else {
                if (arr_size < 10) {
                    rgbd_array[arr_size] = rgbd2_create(_input.mouse_x, _input.mouse_y, 100, 0, 50, 50);
                    arr_size++;
                }
            }

        }

        if (_input.mouse_left_released) {
            pos_held = -1;
        }

        if (pos_held != -1) {
            //for (int i = 0; i < arr_size; i++) {
                //if (i == pos_held) {
                    rgbd_array[pos_held].vel = vec2_mult(rgbd_array[pos_held].vel, (vec2){0, 0});
                    rgbd_array[pos_held].pos.x = _input.mouse_x;// - rgbd_array[pos_held].width / 2;
                    rgbd_array[pos_held].pos.y = _input.mouse_y;// - rgbd_array[pos_held].height / 2;
                //}
            //}
        }

        update_rgbd(rgbd_array, arr_size, pos_held);
        draw(&_renderer, rgbd_array, arr_size);
    }

    free(rgbd_array);
    renderer_delete(&_renderer);
    return 0;

}