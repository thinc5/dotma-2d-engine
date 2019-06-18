#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>

#include "../managers/assetstack.h"
#include "../managers/entitymanager.h"
#include "../entities/entity.h"

/**
 * What is the context of this "scene".
 */
typedef enum SceneType {
    ExcMenu,
    MainMenu,
    TileSelect,
    Battle
} SceneType;

/**
 * Scene specific assets, entities,
 * the type of scene.
 */
typedef struct Scene {
    SceneType type;
    AssetStack assets;
    EntityManager entities;
    // Need to cast as GameData
    void (*event_handler)(void* gameData, void* scene);
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
 * Load a new scene and replace current scene.
 */
void replace_scene(Scene* current, Scene* next);

#endif
