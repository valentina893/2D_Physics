// main.c

#include "engine.h"

#define window_width 640
#define window_height 480

#define max_rgbd 50

int main() {
    engine engine = engine_create("physics", window_width, window_height, max_rgbd);
    engine_run(&engine);
    engine_delete(&engine);
    return 0;
}