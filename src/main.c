#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../include/config.h"
#include "../include/frameratemanager.h"
#include "../include/texturemanager.h"

/**
 * Entry point for Engine.
 */
int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    // Init sdl
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    // Create the window
    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE);
    // Create the renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    // Set default background colour
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Custom framerate manager
    FrameRateManager f = {
        .startTime = 0,
        .endTime = 0,
        .delta = 0,
        .fps = FRAME_CAP,
        .timePerFrame = 1000 / FRAME_CAP // 1000 / prefered fps
    };

    // Custom texture registry
    TextureRegistry tr;
    tr.currentSize = 0;
    tr.totalSize = 10;
    tr.registry = (RegisteredTexture*) malloc(sizeof(RegisteredTexture*) * tr.totalSize);

    // Load a test texture.
    if (!loadTexture(renderer, "./res/cat.jpg", &tr)) {
        freeTextures(&tr);
        printf("Failed to load texture..");
        SDL_Quit();
        return 2;
    }

    if (!loadTexture(renderer, "./res/alex.jpg", &tr)) {
        freeTextures(&tr);
        printf("Failed to load texture..");
        SDL_Quit();
        return 2;
    }

    int pickedTex = 0;

    // Main game loop
    bool game = true;
    while (game) {
        setStartTime(&f);
        while (SDL_PollEvent(&event)) {
            // If person wants to exit i.e. alt+f4 or clicking x
            if (event.type == SDL_QUIT) {
                // Clean up memory here and break main game loop
                printf("Caught SDL_QUIT, exiting now...\n");
                freeTextures(&tr);
                game = false;
                continue;
            }

            // Picked texture
            if (pickedTex + 1 < tr.currentSize) {
                pickedTex++;
            } else {
                pickedTex = 0;
            }

            // Draw
            SDL_RenderClear(renderer);
            if (SDL_RenderCopy(renderer, (&tr)->registry[pickedTex].texture, NULL, NULL) == -1) {
                printf("Failed to render texture..");
            }
            SDL_RenderPresent(renderer);
        showFPS(&f);
        setEndTime(&f);
        }
    }

    SDL_Quit();
    return 0;
}
