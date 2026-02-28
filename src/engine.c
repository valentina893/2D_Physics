// engine.c

#include "engine.h"

engine engine_create(
    char* window_title,
    int window_width,
    int window_height,
    int max_rgbd
) {
    
    engine engine;
    renderer_create(&engine._renderer, window_title, window_width, window_height);

    engine._input = input_create();

    engine.rgbd_array = (rgbd2*)malloc(sizeof(rgbd2) * max_rgbd);
    engine.arr_size = 0;
    engine.max_rgbd = max_rgbd;
    engine.clicked_idx = -1;

    engine.window_width = window_width;
    engine.window_height = window_height;

    engine.running = 1;

    return engine;

}

void engine_run(engine* engine) {

    while (engine->running) {
        
        input_read(&engine->_input, &engine->running);
    
        if (engine->_input.mouse_left_pressed) {
           _engine_onLeftClick(engine);
        }

        if (engine->_input.mouse_left_released) {
           _engine_onLeftRelease(engine);
        }

        _engine_updateRgbdArr(engine);

        _engine_present(engine);

    }

    return;

}

void engine_delete(engine* engine) {

    free(engine->rgbd_array);
    renderer_delete(&engine->_renderer);

    return;

}

void _engine_onLeftClick(engine* engine) {

    for (int i = 0; i < engine->arr_size; i++) {
        if (
            engine->_input.mouse_x >= engine->rgbd_array[i].pos.x - engine->rgbd_array[i].width/2 &&
            engine->_input.mouse_x <= engine->rgbd_array[i].pos.x + engine->rgbd_array[i].width/2 &&
            engine->_input.mouse_y >= engine->rgbd_array[i].pos.y - engine->rgbd_array[i].height/2 &&
            engine->_input.mouse_y <= engine->rgbd_array[i].pos.y + engine->rgbd_array[i].height/2
        ) {
            engine->rgbd_array[i].inv_mass = 0;
            engine->clicked_idx = i;
            break;
        }
    }


    if (engine->clicked_idx == -1 && engine->arr_size < engine->max_rgbd) {

        engine->rgbd_array[engine->arr_size] = rgbd2_create(engine->_input.mouse_x, engine->_input.mouse_y, 100.0f, 0, 50, 50);
        engine->arr_size++;

    }

    return;

}

void _engine_onLeftRelease(engine* engine) {

    if (engine->clicked_idx != -1) {
        engine->rgbd_array[engine->clicked_idx].inv_mass = 1.0f / engine->rgbd_array[engine->clicked_idx].mass;
    }
    engine->clicked_idx = -1;

    return;

}

void _engine_updateRgbdArr(engine* engine) {

    if (engine->clicked_idx != -1) {
        engine->rgbd_array[engine->clicked_idx].pos.x = engine->_input.mouse_x;
        engine->rgbd_array[engine->clicked_idx].pos.y = engine->_input.mouse_y;
    }

    for (int i = 0; i < engine->arr_size; i++) {
        if (engine->rgbd_array[i].inv_mass != 0) {
            rgbd2_applyForce(&engine->rgbd_array[i], (vec2){0, gravity * engine->rgbd_array[i].mass}, (vec2){0, 0});
            rgbd2_integrateEuler(&engine->rgbd_array[i], delta_time);
            rgbd2_windowCollision(&engine->rgbd_array[i], engine->window_width+(50/2), engine->window_height+(50/2));
        }
    }

    for (int i = 0; i < engine->arr_size-1; i++) {
        for (int j = 1; j < engine->arr_size; j++) {
            rgbd2_objectCollision(&engine->rgbd_array[i], &engine->rgbd_array[j]);
        }
    }

    return;

}

void _engine_present(engine* engine) {

    renderer_clear(&engine->_renderer, 0, 0, 0, 0);
        
    for (int i = 0; i < engine->arr_size; i++) {
        renderer_drawRect(
            &engine->_renderer, 
            engine->rgbd_array[i].pos.x-(engine->rgbd_array[i].width/2), 
            engine->rgbd_array[i].pos.y-(engine->rgbd_array[i].height/2), 
            engine->rgbd_array[i].width, engine->rgbd_array[i].height, 
            100, 100, 100, 0
        );
    }
        
    renderer_present(&engine->_renderer);

    return;

}