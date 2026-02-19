// renderer.h

#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

// Basic renderer struct to handle low-level SDL components.
typedef struct renderer {

    SDL_Window* window;
    SDL_Renderer* _renderer;
    
} renderer;

// Initializes renderer with needed window information.
// - Returns 1 if successful initialization, 0 if unsuccessful.
int renderer_create(renderer* renderer, char* window_title, int window_width, int window_height);

// Resets screen color and clears all rendered pixels.
void renderer_clear(renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

// Draws one texture to specified position and size on the window.
void renderer_drawTexture(renderer* renderer, SDL_Texture* texture, int x_pos, int y_pos, int w, int h);

// Draws a rectangle to specified position and size on the window. Color must be selected.
void renderer_drawRect(renderer* renderer, int x_pos, int y_pos, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

// Draws a circle outline to specified position and size on the window. Color must be selected.
void renderer_drawHollowCircle(renderer* renderer, int x_pos, int y_pos, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

// Fills a circle outline with specified color.
void renderer_drawFilledCircle(renderer* renderer, int x_pos, int y_pos, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

// Wrapper function for presenting the drawn frame.
void renderer_present(renderer* renderer);

// Cleans up renderer struct.
void renderer_delete(renderer* renderer);

#endif
