// engine.h

#ifndef ENGINE_H
#define ENGINE_H

#include "input.h"
#include "renderer.h"
#include "rgbd2.h"

typedef struct engine {

    renderer _renderer;
    input _input;

    rgbd2* rgbd_array;
    int arr_size;
    int max_rgbd;

    int running;

    int window_width, window_height;

} engine;

/*
Initializes engine class and maximum rigidbodies to be handled in simulation.
*/
engine engine_create(
    char* window_title,
    int window_width,
    int window_height,
    int max_rgbd
);

/*
Runs all processes of the physics engine.
*/
void engine_run(engine* engine);

/*
Cleans up all elements of engine class.
*/
void engine_delete(engine* engine);

#endif