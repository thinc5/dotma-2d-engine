#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>

#include "game.h"

/**
 * Render all renderable entites.
 */
void renderEntities(GameData* gameData);

#endif