#include <stdlib.h>
#include <string.h>

#include "../../include/debug.h"
#include "../../include/game.h"
#include "../../include/managers/assetstack.h"
#include "../../include/managers/asset.h"

/**
 * Initialize the asset stack.
 */
bool init_asset_stack(AssetStack* stack) {
    stack->allocations = -1;
    stack->heads = NULL;
    stack->tail = NULL;
    return true;
}

/**
 * Push and asset onto the stack.
 */
static bool push_asset(SDL_Renderer* renderer, AssetStack* stack, const char* asset_path) {
    // If we dont have a node at our current head make one.
    if (stack->heads[stack->allocations] == NULL) { 
        stack->heads[stack->allocations] = (AssetNode*) malloc(sizeof(AssetNode));
        stack->tail = stack->heads[stack->allocations];
    } else {
        // We need to allocate memory for the current head->next.
        stack->tail->next = (AssetNode*) malloc(sizeof(AssetNode));
        stack->tail = stack->tail->next;
    }
    // Set the tail's next node to NULL.
    stack->tail->next = NULL;
    AssetNode* node = stack->tail;
    // Allocate space for the asset.
    node->asset = (RegisteredAsset*) malloc(sizeof(RegisteredAsset));
    // Attempt to load the asset.
    if (!type_asset(node->asset, asset_path)) {
        ERROR_LOG("Unable to type asset %s.\n", asset_path);
        return false;
    }
    switch (node->asset->type) {
        case Texture:
            node->asset->pointer.texture = IMG_LoadTexture(renderer, asset_path);
            if (node->asset->pointer.texture == NULL) {
                return false;
            }
            break;
        case Font:
            node->asset->pointer.font = TTF_OpenFont(asset_path, 20);
            if (node->asset->pointer.font == NULL) {
                return false;
            }
            break;
        case Sound:
            node->asset->pointer.sound = Mix_LoadWAV(asset_path);
            if (node->asset->pointer.font == NULL) {
                return false;
            }
            break;
        default:
            return false;
    }
    return true;
}

/**
 * Push a chunk of assets onto the asset stack as per provided asset manifest.
 */
bool push_asset_chunk(SDL_Renderer* renderer, AssetStack* stack, const char* manifest) {
    // Check if the file can be opened.
    char asset_path[100];   // Maximum size of provided filename.
    FILE* fp = fopen(manifest, "r");
    if (fp == NULL) {
        return false;
    }
    // Is this the first allocation for the stack?
    if (stack->allocations == -1) {
        stack->heads = (AssetNode**) malloc(sizeof(AssetNode*));
    // Increase the size of the head array.
    } else if  (stack->allocations < 0) {
        stack->heads = (AssetNode**) realloc(stack->heads, (stack->allocations + 1) * sizeof(AssetNode*));
    }
    stack->allocations++;
    // We dont have a head for the most recent 
    stack->heads[stack->allocations] = NULL;
    INFO_LOG("Stack allocations: %d\n", stack->allocations);
    // Read each line of the file and add a new node for each loaded asset.
    while(fgets(asset_path, sizeof(asset_path), fp)) {
        int blen = strlen(asset_path);
        if (asset_path[blen - 1] == '\n') {
            asset_path[strlen(asset_path) - 1] = '\0';
        }
        if (!push_asset(renderer, stack, asset_path)) {
            // Unable to load?
            ERROR_LOG("Could not load file: %s\n", asset_path);
        }
        memset(asset_path, '\0', sizeof(asset_path));
    }
    // Close file.
    fclose(fp);
    for (int i = 0; i < stack->allocations + 1; i++) {
        INFO_LOG("head: %lu ", (unsigned long) stack->heads[i]);
    }
    INFO_LOG("\n");
    return true;
}

/**
 * Pop a chunk of the assets from the stack.
 */
bool pop_asset_chunk(AssetStack* stack) {
    // Free all assets from the top most chunk.
    INFO_LOG("Free head: %lu\n", (unsigned long) stack->heads[stack->allocations]);
    AssetNode* current = stack->heads[stack->allocations];
    while(current != NULL) {
        // INFO_LOG("Freeing %s\n", current->asset->reference);
        // Free the asset within the RegisteredAsset.
        free_asset(current->asset);
        // Free the RegisteredAsset.
	    free(current->asset);
        // Get the pointer to the next asset.
        current->asset = NULL;
        AssetNode* temp = current->next;
        // Free the AssetNode.
        free(current);
        // Set our temp as our next asset.
        current = temp;
    }
    // Shrink the array of heads.
    stack->allocations--;
    INFO_LOG("New allocations: %d\n", stack->allocations);
    // Is it greater than 1 (do we even have any left?)
    if (stack->allocations > -1) {
        stack->heads = (AssetNode**) realloc(stack->heads, (stack->allocations + 1) * sizeof(AssetNode*));
        // Set the tail pointer.
        AssetNode* tail = stack->heads[stack->allocations];
        while(tail->next != NULL) {
            tail = tail->next;
        };
    } else {
        // Free all data.
        free(stack->heads);
        stack->heads = NULL;
        stack->tail = NULL;
    }
    return true;
}

/**
 * Free all the assets on the stack.
 */
bool free_asset_stack(AssetStack* stack) {
    while(stack->allocations > 0) {
        pop_asset_chunk(stack);
    }
    return true;
}

/**
 * If an asset is found with provided reference, return pointer to asset, else return NULL.
 */
RegisteredAsset* get_asset_by_ref(const char* reference, int chunk) {
    // If the provided chunk is out of our range we return NULL.
    if (chunk < 0 || chunk > gameData.assets.allocations) {
        return NULL;
    }
    AssetNode* node = gameData.assets.heads[chunk];
    while (node != NULL) {
        if (strcmp(node->asset->reference, reference) == 0) {
            return (RegisteredAsset*) node->asset;
	    }
        node = node->next;
    }
    return NULL;
}

#ifdef DEBUG
/**
 * Linearly traverse the stack for debug purposes.
 */
void debug_asset_stack(AssetStack stack) {
    INFO_LOG("Debugging stack.\n");
    INFO_LOG("Allocations: %d\n", stack.allocations);
    if (stack.heads[0] == NULL) {
        INFO_LOG("We don't have a head..\n");
	return;
    }
    INFO_LOG("We have a head!\n");
    AssetNode* start = stack.heads[0];
    while (start != NULL) {
	INFO_LOG("Texture: %s\n", start->asset->reference);
        start = start->next;
    }
}
#endif

