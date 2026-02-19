#include "renderer.h"
#include "rgbd2.h"

void physics(rgbd2 *rgbd) {
    rgbd2_applyForce(rgbd, 0, 980.0f * rgbd->mass);
    rgbd2_integrateEuler(rgbd, (float)1/60);
    rgbd2_windowCollision(rgbd, 640, 480);
}

int main(int argc, char **argv) {

    if (argc < 4) {
        printf("pass in rgbd vals!\n");
        return 0;
    }

    renderer renderer;

    if (renderer_create(&renderer, "test", 640, 480) == 0) {
        printf("failure creating renderer");
    }

    rgbd2 a = rgbd2_create(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), 50, 50);
    a.restitution = 0.8f;
    rgbd2 b = rgbd2_create(100, 300, 100, 50, 50);

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
            }
        }
        physics(&a);
        //physics(&b);
        rgbd2_objectCollision(&a, &b);
        renderer_clear(&renderer, 0, 0, 0, 0);
        renderer_drawRect(&renderer, a.pos.x, a.pos.y, a.width, a.height, 255, 0, 0, 0);
        renderer_drawRect(&renderer, b.pos.x, b.pos.y, b.width, b.height, 0, 255, 0, 0);
        renderer_present(&renderer);
    }

    renderer_delete(&renderer);

    return 0;

}