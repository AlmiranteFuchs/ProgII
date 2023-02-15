// Author: B. Fuchs

#ifndef GAME_GRAPHICS_H
#define GAME_GRAPHICS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "gameObjects.h"
#include "gameLogic.h" // Just for the board size

// Enum for bitmap paths, and font lol
#define BITMAP_MAX 20
typedef enum
{
    GEM_0,
    GEM_1,
    GEM_2,
    GEM_3,
    GEM_4,
    GEM_5,
    GEM_6,
    ITEM_10,
    EMPTY_8,
    EMPTY_9,
    UI_MAPFRAME_11,
    UI_BACKGROUND_12,
    UI_FOREGROUND_13,
    F_TERRARIA_14,
    C_CLOUD_0,
    C_CLOUD_1,

} BitmapPath;

static char *const bitmapPaths[] =
    {
        [GEM_0] = "resources/sprites/terraria_sprites/gems/Gem_0.png",
        [GEM_1] = "resources/sprites/terraria_sprites/gems/Gem_1.png",
        [GEM_2] = "resources/sprites/terraria_sprites/gems/Gem_2.png",
        [GEM_3] = "resources/sprites/terraria_sprites/gems/Gem_3.png",
        [GEM_4] = "resources/sprites/terraria_sprites/gems/Gem_4.png",
        [GEM_5] = "resources/sprites/terraria_sprites/gems/Gem_5.png",
        [GEM_6] = "resources/sprites/terraria_sprites/gems/Gem_6.png",
        [ITEM_10] = "resources/sprites/terraria_sprites/gems/Item_21.png",
        [UI_MAPFRAME_11] = "resources/sprites/terraria_sprites/UI/MapFrame.png",
        [UI_BACKGROUND_12] = "resources/sprites/terraria_sprites/UI/Splash_9_0.png",
        [UI_FOREGROUND_13] = "resources/sprites/terraria_sprites/UI/foreground.png",
        [F_TERRARIA_14] = "resources/fonts/Terraria-Font/ANDYB.TTF",
        [C_CLOUD_0] = "resources/sprites/terraria_sprites/cenary/clouds/Cloud_0.png"};

// Draw the tiles, receives a matrix of tiles
void drawTiles(GameManager *gm);
void drawTile(Tile tile, GameManager *gm);

// Load the bitmaps
void initGraphics(GameManager *gm);

// Draw UI
void drawUI(GameManager *gm);

// Draw background
void drawBackground(GameManager *gm);

void drawMinigame(GameManager *gm);

#endif // GAME_GRAPHICS_H