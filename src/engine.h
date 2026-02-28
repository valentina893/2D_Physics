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
    int clicked_idx;

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

/*
Action handler for left click. Can spawn or select a rigidbody.
*/
void _engine_onLeftClick(engine* engine);

/*
Action handler for left release. Will reset selected rigidbody's inv_mass and/or clicked_idx
*/
void _engine_onLeftRelease(engine* engine);

/*
Updates all existing rigidbodies in simulation. Skips integration method if inv_mass is 0.
*/
void _engine_updateRgbdArr(engine* engine);

/*
Draws current frame of the simulation.
*/
void _engine_present(engine* engine);

#endif