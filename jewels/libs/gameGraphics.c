
#include "gameGraphics.h"
#include <stdio.h>
#include <string.h>

void initGraphics(GameManager *gm)
{
    gm->bitmaps[0] = al_load_bitmap(bitmapPaths[GEM_0]);
    gm->bitmaps[1] = al_load_bitmap(bitmapPaths[GEM_1]);
    gm->bitmaps[2] = al_load_bitmap(bitmapPaths[GEM_2]);
    gm->bitmaps[3] = al_load_bitmap(bitmapPaths[GEM_3]);
    gm->bitmaps[4] = al_load_bitmap(bitmapPaths[GEM_4]);
    gm->bitmaps[5] = al_load_bitmap(bitmapPaths[GEM_5]);
    gm->bitmaps[6] = al_load_bitmap(bitmapPaths[GEM_6]);
    gm->bitmaps[7] = al_load_bitmap(bitmapPaths[ITEM_10]);

    // UI
    gm->bitmaps[10] = al_load_bitmap(bitmapPaths[UI_MAPFRAME_11]);
    gm->bitmaps[11] = al_load_bitmap(bitmapPaths[UI_BACKGROUND_12]);
    gm->bitmaps[12] = al_load_bitmap(bitmapPaths[UI_FOREGROUND_13]);
    gm->bitmaps[14] = al_load_bitmap(bitmapPaths[C_CLOUD_0]);

    // Font, i'm not renaming this function, but it loads graphical stuff anyway
    gm->font = al_load_ttf_font("resources/fonts/Terraria-Font/ANDYB.TTF", 40, 0);

    // gif
    gm->gifmaps[0] = algif_load_animation("resources/sprites/terraria_sprites/minigame/player.gif");
}

void drawTile(Tile tile, GameManager *gm)
{
    //  Testing only, draw a rectangle
    // int tile_s = TILE_SIZE;
    // al_draw_filled_rectangle(tile.transform.x, tile.transform.y, tile.transform.x + tile_s, tile.transform.y + tile_s, al_map_rgb(255, 0, 0));

    if (tile.transform.visible == 0)
        return;

    // If selected, draw bitmap bigger
    if (tile.selected == 1)
    {
        al_draw_scaled_bitmap(gm->bitmaps[tile.sprite.sprite_num], 0, 0, TILE_SIZE, TILE_SIZE, tile.transform.x - 2, tile.transform.y - 2, 45, 45, 0);
        return;
    }

    // Tile has a value, draw it
    if (tile.sprite.sprite_num != -1)
    {
        // Draw the tile using bitmap
        al_draw_bitmap(gm->bitmaps[tile.sprite.sprite_num], tile.transform.x, tile.transform.y, 0);
    }
}

void drawTiles(GameManager *gm)
{
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {
            drawTile(gm->board[i][j], gm);
        }
    }
}

void drawBackground(GameManager *gm)
{
    // Far background
    al_draw_bitmap(gm->bitmaps[UI_BACKGROUND_12], 0, 0, 0);

    // For all the cenary clouds
    for (int i = 0; i < CENARY_OBJS; i++)
    {
        // gm->foregrounds[i]
        if (gm->cenaryObjects[i].Transform.visible == 1 && gm->cenaryObjects[i].cenary_type == CENARY_CLOUDS)
        {
            al_draw_bitmap(gm->bitmaps[gm->cenaryObjects[i].sprite.sprite_num], gm->cenaryObjects[i].Transform.x, gm->cenaryObjects[i].Transform.y, 0);
        }
    }

    // For all the foreground
    for (int i = 0; i < FOREGROUNDS; i++)
    {
        // gm->foregrounds[i]
        if (gm->foregrounds[i].Transform.visible == 1)
        {
            al_draw_bitmap(gm->bitmaps[UI_FOREGROUND_13], gm->foregrounds[i].Transform.x, gm->foregrounds[i].Transform.y, 0);
        }
    }

     // For all the cenary object
     for (int i = 0; i < CENARY_OBJS; i++)
    {
        // gm->foregrounds[i]
        if (gm->cenaryObjects[i].Transform.visible == 1 && gm->cenaryObjects[i].cenary_type == CENARY_OBJECTS)
        {
            al_draw_bitmap(gm->bitmaps[gm->cenaryObjects[i].sprite.sprite_num], gm->cenaryObjects[i].Transform.x, gm->cenaryObjects[i].Transform.y, 0);
        }
    }
}

void drawUI(GameManager *gm)
{
    // Draw the UI, scale to 2x
    // al_draw_scaled_bitmap(bitmaps[6], 0, 0, 256, 263, 0, 0, (SCREEN_WIDTH/2)+ 100, SCREEN_HEIGHT+35 , 0);
    al_draw_bitmap(gm->bitmaps[10], 0, 0, 0);

    // Draw the score
    // Position is hardcoded, but it's just a test
    int score = gm->score;
    char score_str[10];

    char *score_str_format = "Score: %d";
    sprintf(score_str, score_str_format, score);
    al_draw_text(gm->font, al_map_rgb(255, 255, 255), (SCREEN_WIDTH / 2) * 1.5, 0, 0, score_str);

    // Draw the turn
    char turn_str[10];
    char *turn_str_format = "Turn: %d";
    sprintf(turn_str, turn_str_format, gm->turn);
    al_draw_text(gm->font, al_map_rgb(255, 255, 255), (SCREEN_WIDTH / 2) * 1.5, 50, 0, turn_str);

    // Draw time
    char time_str[10];
    char *time_str_format = "Time: %d";
    int time_to_int = (int)gm->time;
    sprintf(time_str, time_str_format, time_to_int);
    al_draw_text(gm->font, al_map_rgb(255, 255, 255), (SCREEN_WIDTH / 2) * 1.5, 100, 0, time_str);
}

void drawMinigame(GameManager *gm){
    al_draw_bitmap(algif_get_bitmap(gm->gifmaps[0], al_get_time()), 1000, 505, 0);
}