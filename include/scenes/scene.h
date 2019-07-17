#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>

#include <stdbool.h>

#include "../managers/assetstack.h"
#include "../managers/quadtree.h"
#include "../entities/entity.h"

/**
 * What is the context of this "scene".
 */
typedef enum SceneType {
    EscMenu,
    MainMenu,
    Debug,
    TileSelect,
    Battle
} SceneType;

/**
 * Scene abstraction, each game "view" is considered a scene.
 */
typedef struct Scene {
    // Type of scene.
    SceneType type;
    // Entities present in scene.
    QuadTree entities;
    // The scene specific event handler.
    void (*event_handler)();
    // Optional components of the scene.
    SDL_Texture* bg;
    SDL_Texture* cursor;
    // Scene specific state.
    void* state;
} Scene;

/**
 * Initialize the scene components.
 */
bool init_scene(Scene* scene);

/**
 * Free a scene.
 */
void free_scene(Scene* scene);

 /**
  * Switch scenes.
  */
void change_scene(void (*next)(void));

#endif

