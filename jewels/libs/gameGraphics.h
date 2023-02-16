// Author: B. Fuchs

#ifndef GAME_GRAPHICS_H
#define GAME_GRAPHICS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "gameLogic.h"
#include "gameObjects.h"

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
    UI_MAPFRAME_0,
    UI_BACKGROUND_0,
    UI_FOREGROUND_0,
    F_TERRARIA_0,
    C_CLOUD_0,
    C_CLOUD_1,
    BOSS_0

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
        [UI_MAPFRAME_0] = "resources/sprites/terraria_sprites/UI/MapFrame.png",
        [UI_BACKGROUND_0] = "resources/sprites/terraria_sprites/UI/Splash_9_0.png",
        [UI_FOREGROUND_0] = "resources/sprites/terraria_sprites/UI/foreground.png",
        [F_TERRARIA_0] = "resources/fonts/Terraria-Font/ANDYB.TTF",
        [C_CLOUD_0] = "resources/sprites/terraria_sprites/cenary/clouds/Cloud_0.png",
        [C_CLOUD_1] = "resources/sprites/terraria_sprites/cenary/clouds/Cloud_1.png",
        [BOSS_0] = "resources/sprites/terraria_sprites/minigame/NPC_35.png"};

// Draw the tiles, receives a matrix of tiles
void drawTiles(GameManager *gm);
void drawTile(Tile tile, GameManager *gm);

// Load the bitmaps
void initGraphics(GameManager *gm);

// Destroy the bitmaps
void destroyGraphics(GameManager *gm);

// Draw UI
void drawUI(GameManager *gm);

// Draw background
void drawBackground(GameManager *gm);

void drawMinigame(GameManager *gm);

// Draw GameOver
void drawGameOver(GameManager *gm);

#endif // GAME_GRAPHICS_H