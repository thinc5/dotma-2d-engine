#include <SDL2/SDL.h>

#include <stdlib.h>
#include <stdbool.h>

#include "../include/config.h"
#include "../include/game.h"
#include "../include/timer.h"
#include "../include/texturemanager.h"
#include "../include/fontmanager.h"

void initGame(GameData* gameData) {
    gameData->status = true;
    // Create the window
    gameData->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE),
    // Create the renderer
    gameData->renderer = SDL_CreateRenderer(gameData->window, -1, SDL_RENDERER_ACCELERATED);
    // Set default background colour
    SDL_SetRenderDrawColor(gameData->renderer, 255, 0, 0, 255);
    // Custom framerate manager
    gameData->fps = (FrameRateManager*) malloc(sizeof(FrameRateManager));
    initFPSManager(gameData->fps);
    // Custom texture registry
    gameData->tr = (TextureRegistry*) malloc(sizeof(TextureRegistry));
    gameData->tr->currentSize = 0;
    gameData->tr->totalSize = 0;
    // Custom font registry
    gameData->fr = (FontRegistry*) malloc(sizeof(FontRegistry));
    gameData->fr->currentSize = 0;
    gameData->fr->totalSize = 0;
}

/**
 * Free game data struct.
 */
void freeGame(GameData* game) {
    printf("Freeing textures...\n");
    freeTextures(game->tr);
    free(game->tr->registry);
    game->tr->registry = NULL;
    free(game->tr);
    game->tr = NULL;
    printf("Freeing fonts..\n");
    freeFonts(game->fr);
    free(game->fr->registry);
    game->fr->registry = NULL;
    free(game->fr);
    game->fr = NULL;
    printf("Freeing timer\n");
    free(game->fps);
    printf("Freeing renderer\n");
    SDL_DestroyRenderer(game->renderer); // Causes SEGFAULT?
    printf("Freeing window\n");
    SDL_DestroyWindow(game->window);
}
