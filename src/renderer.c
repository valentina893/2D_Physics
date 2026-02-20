// renderer.c

#include <stdio.h>

#include "renderer.h"

int renderer_create(renderer* renderer, char* window_title, int window_width, int window_height) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 0;
    }

    renderer->window = SDL_CreateWindow(
        window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width, window_height, SDL_WINDOW_SHOWN
    );

    SDL_RaiseWindow(renderer->window); // for mac

    if (renderer->window == NULL) {
        printf("Error creating SDL_Window*: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

    renderer->_renderer = SDL_CreateRenderer(renderer->window, -1, render_flags);

    if (renderer->_renderer == NULL) {
        printf("Error creating SDL_Renderer*: %s\n", SDL_GetError());
        SDL_DestroyWindow(renderer->window);
        SDL_Quit();
        return 0;
    }

    return 1;

}

void renderer_clear(renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {

    SDL_SetRenderDrawColor(renderer->_renderer, r, g, b, a);
    SDL_RenderClear(renderer->_renderer);

}

void renderer_drawTexture(renderer* renderer, SDL_Texture* texture, int x_pos, int y_pos, int w, int h) {

    if (texture == NULL || renderer == NULL) return;

    SDL_Rect rect = {x_pos, y_pos, w, h};

    SDL_RenderCopy(renderer->_renderer, texture, NULL, &rect);

    return;

}

void renderer_drawRect(renderer* renderer, int x_pos, int y_pos, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {

    if (renderer == NULL) return;

    SDL_Rect rect = {x_pos, y_pos, w, h};

    SDL_SetRenderDrawColor(renderer->_renderer, r, g, b, a);
    SDL_RenderFillRect(renderer->_renderer, &rect);

    return;

}

void renderer_drawHollowCircle(renderer* renderer, int x_pos, int y_pos, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {

    SDL_SetRenderDrawColor(renderer->_renderer, r, g, b, a);

    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (r << 1);

    while (x >= y) {
        SDL_RenderDrawPoint(renderer->_renderer, x_pos + x, y_pos + y);
        SDL_RenderDrawPoint(renderer->_renderer, x_pos + y, y_pos + x);
        SDL_RenderDrawPoint(renderer->_renderer, x_pos - y, y_pos + x);
        SDL_RenderDrawPoint(renderer->_renderer, x_pos - x, y_pos + y);
        SDL_RenderDrawPoint(renderer->_renderer, x_pos - x, y_pos - y);
        SDL_RenderDrawPoint(renderer->_renderer, x_pos - y, y_pos - x);
        SDL_RenderDrawPoint(renderer->_renderer, x_pos + y, y_pos - x);
        SDL_RenderDrawPoint(renderer->_renderer, x_pos + x, y_pos - y);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }

}

void renderer_drawFilledCircle(renderer* renderer, int x_pos, int y_pos, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {

    SDL_SetRenderDrawColor(renderer->_renderer, r, g, b, a);

    for (int y = -radius; y <= radius; y++) {
        int dx = (int)SDL_sqrt(radius * radius - y * y);
        int x1 = x_pos - dx;
        int x2 = x_pos + dx;

        SDL_RenderDrawLine(renderer->_renderer, x1, y_pos + y, x2, y_pos + y);
    }

}

void renderer_present(renderer* renderer) {
    SDL_RenderPresent(renderer->_renderer);
}

void renderer_delete(renderer* renderer) {

    if (renderer == NULL) return;

    SDL_DestroyRenderer(renderer->_renderer);
    SDL_DestroyWindow(renderer->window);
    SDL_Quit();
    return;
    
}