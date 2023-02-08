// Autor: B. Fuchs
// Game objects header file

#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include "gameComponents.h"

// Game objects
// Jewels tile



// Enum Bitmaps for path
enum Bitmaps
{
    GEM_0,
    GEM_1,
    GEM_2,
    GEM_3,
    GEM_4,
    GEM_5,
    GEM_6,
    ITEM_21, // Empty space
};

static char *const bitmapPaths[] =
    {
        [GEM_0] = "resources/sprites/terraria_sprites/Gem_0.png",
        [GEM_1] = "resources/sprites/terraria_sprites/Gem_1.png",
        [GEM_2] = "resources/sprites/terraria_sprites/Gem_2.png",
        [GEM_3] = "resources/sprites/terraria_sprites/Gem_3.png",
        [GEM_4] = "resources/sprites/terraria_sprites/Gem_4.png",
        [GEM_5] = "resources/sprites/terraria_sprites/Gem_5.png",
        [GEM_6] = "resources/sprites/terraria_sprites/Gem_6.png",
        [ITEM_21] = "resources/sprites/terraria_sprites/Item_21.png"
};

// Represents a single tile, a jewel
#define TILE_SIZE 40
typedef struct
{
    Transform transform;
    Sprite sprite;
    int value;
    int falling;
    int selected;
    int real_posX;
    int real_posY;
} Tile;





#endif // GAMEOBJECTS_H