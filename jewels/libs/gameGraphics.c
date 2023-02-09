
#include "gameGraphics.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <string.h>

// Array of bitmaps
static ALLEGRO_BITMAP *bitmaps[15];
static ALLEGRO_FONT *font;

void loadBitmaps()
{
    bitmaps[0] = al_load_bitmap("resources/sprites/terraria_sprites/Gem_0.png");
    bitmaps[1] = al_load_bitmap("resources/sprites/terraria_sprites/Gem_1.png");
    bitmaps[2] = al_load_bitmap("resources/sprites/terraria_sprites/Gem_2.png");
    bitmaps[3] = al_load_bitmap("resources/sprites/terraria_sprites/Gem_3.png");
    bitmaps[4] = al_load_bitmap("resources/sprites/terraria_sprites/Gem_4.png");
    bitmaps[5] = al_load_bitmap("resources/sprites/terraria_sprites/Gem_5.png");
    bitmaps[6] = al_load_bitmap("resources/sprites/terraria_sprites/Gem_6.png");
    bitmaps[7] = al_load_bitmap("resources/sprites/terraria_sprites/Item_21.png");

    // UI
    bitmaps[10] = al_load_bitmap("resources/sprites/terraria_sprites/UI/MapFrame.png");
    bitmaps[11] = al_load_bitmap("resources/sprites/terraria_sprites/UI/Splash_9_0.png");

    // Font, i'm not renaming this function, but it loads graphical stuff anyway
    font = al_load_ttf_font("resources/fonts/Terraria-Font/ANDYB.TTF", 40, 0);
}

void drawTile(Tile tile)
{
    // printf("Drawing tile at %d, %d\n", tile.transform.x, tile.transform.y);
    //  Testing only, draw a rectangle
    // int tile_s = TILE_SIZE;
    // al_draw_filled_rectangle(tile.transform.x, tile.transform.y, tile.transform.x + tile_s, tile.transform.y + tile_s, al_map_rgb(255, 0, 0));

    if (tile.transform.visible == 0)
        return;

    // If selected, draw bitmap bigger
    if (tile.selected == 1)
    {
        al_draw_scaled_bitmap(bitmaps[tile.sprite.sprite_num], 0, 0, TILE_SIZE, TILE_SIZE, tile.transform.x - 2, tile.transform.y - 2, 45, 45, 0);
        return;
    }

    // Tile has a value, draw it
    if (tile.sprite.sprite_num != -1)
    {
        // Draw the tile using bitmap
        al_draw_bitmap(bitmaps[tile.sprite.sprite_num], tile.transform.x, tile.transform.y, 0);
    }
}

void drawTiles(Tile tiles[BOARD_WIDTH][BOARD_HEIGHT])
{
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {
            drawTile(tiles[i][j]);
        }
    }
}

void drawBackground()
{
    al_draw_bitmap(bitmaps[11], 0, 0, 0);
}

void drawUI(GameManager *gm)
{
    // Draw the UI, scale to 2x
    // al_draw_scaled_bitmap(bitmaps[6], 0, 0, 256, 263, 0, 0, (SCREEN_WIDTH/2)+ 100, SCREEN_HEIGHT+35 , 0);
    al_draw_bitmap(bitmaps[10], 0, 0, 0);

    // Draw the score
    // Position is hardcoded, but it's just a test
    int score = gm->score;
    char score_str[10];

    char *score_str_format = "Score: %d";
    sprintf(score_str, score_str_format, score);


    al_draw_text(font, al_map_rgb(255, 255, 255), (SCREEN_WIDTH/2) * 1.5, 0, 0, score_str);
}