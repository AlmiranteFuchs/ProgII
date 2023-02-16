
#include "gameGraphics.h"
#include <stdio.h>
#include <string.h>


void initGraphics(GameManager *gm)
{
    gm->bitmaps[GEM_0] = al_load_bitmap(bitmapPaths[GEM_0]);
    gm->bitmaps[GEM_1] = al_load_bitmap(bitmapPaths[GEM_1]);
    gm->bitmaps[GEM_2] = al_load_bitmap(bitmapPaths[GEM_2]);
    gm->bitmaps[GEM_3] = al_load_bitmap(bitmapPaths[GEM_3]);
    gm->bitmaps[GEM_4] = al_load_bitmap(bitmapPaths[GEM_4]);
    gm->bitmaps[GEM_5] = al_load_bitmap(bitmapPaths[GEM_5]);
    gm->bitmaps[GEM_6] = al_load_bitmap(bitmapPaths[GEM_6]);

    // UI
    gm->bitmaps[UI_MAPFRAME_0] = al_load_bitmap(bitmapPaths[UI_MAPFRAME_0]);
    gm->bitmaps[UI_BACKGROUND_0] = al_load_bitmap(bitmapPaths[UI_BACKGROUND_0]);
    gm->bitmaps[UI_FOREGROUND_0] = al_load_bitmap(bitmapPaths[UI_FOREGROUND_0]);
    gm->bitmaps[C_CLOUD_0] = al_load_bitmap(bitmapPaths[C_CLOUD_0]);
    gm->bitmaps[C_CLOUD_1] = al_load_bitmap(bitmapPaths[C_CLOUD_1]);
    gm->bitmaps[BOSS_0] = al_load_bitmap(bitmapPaths[BOSS_0]);


    // Font, i'm not renaming this function, but it loads graphical stuff anyway
    gm->font = al_load_ttf_font("resources/fonts/Terraria-Font/ANDYB.TTF", 40, 0);

    // gif, I dont have time to make a gif library, so I'm using this one, path hardcoded bc Is just these 3
    gm->gifmaps[0] = algif_load_animation("resources/sprites/terraria_sprites/minigame/player.gif");
    // Bunny
    gm->gifmaps[1] = algif_load_animation("resources/sprites/terraria_sprites/cenary/bunny_l.gif");
    gm->gifmaps[2] = algif_load_animation("resources/sprites/terraria_sprites/cenary/bunny_r.gif"); 
}

void destroyGraphics(GameManager *gm){
    for (int i = 0; i < BITMAPS_COUNT; i++)
    {
        al_destroy_bitmap(gm->bitmaps[i]);
    }
    for (int i = 0; i < GIFMAPS_COUNT; i++)
    {
        algif_destroy_animation(gm->gifmaps[i]);
    }
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
    al_draw_bitmap(gm->bitmaps[UI_BACKGROUND_0], 0, 0, 0);

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
            al_draw_bitmap(gm->bitmaps[UI_FOREGROUND_0], gm->foregrounds[i].Transform.x, gm->foregrounds[i].Transform.y, 0);
        }
    }

    // For all the cenary object
    for (int i = 0; i < CENARY_OBJS; i++)
    {
        // gm->foregrounds[i]
        if (gm->cenaryObjects[i].Transform.visible == 1 && gm->cenaryObjects[i].cenary_type == CENARY_OBJECTS)
        {
            al_draw_bitmap(algif_get_bitmap(gm->gifmaps[gm->cenaryObjects[i].sprite.sprite_num], al_get_time()), gm->cenaryObjects[i].Transform.x, gm->cenaryObjects[i].Transform.y, 0);
        }
    }
}

void drawUI(GameManager *gm)
{
    // Draw the UI, scale to 2x
    // al_draw_scaled_bitmap(bitmaps[6], 0, 0, 256, 263, 0, 0, (SCREEN_WIDTH/2)+ 100, SCREEN_HEIGHT+35 , 0);
    al_draw_bitmap(gm->bitmaps[UI_MAPFRAME_0], 0, 0, 0);

    // Draw the score
    // Position is hardcoded, but it's just a test
    int score = gm->score;
    char score_str[10];

    char *score_str_format = "Score: %d";
    sprintf(score_str, score_str_format, score);
    al_draw_text(gm->font, al_map_rgb(255, 255, 255), (SCREEN_WIDTH / 2) * 1.5, 0, 0, score_str);

    // Draw the highscore
    int highscore = gm->highscore;
    char highscore_str[50];
    char *highscore_str_format = "Highscore: %d";
    sprintf(highscore_str, highscore_str_format, highscore);
    al_draw_text(gm->font, al_map_rgb(255, 255, 255), (SCREEN_WIDTH / 2) * 1.5, 50, 0, highscore_str);

    // Draw the turn
    char turn_str[10];
    char *turn_str_format = "Turn: %d";
    sprintf(turn_str, turn_str_format, gm->turn);
    al_draw_text(gm->font, al_map_rgb(255, 255, 255), (SCREEN_WIDTH / 2) * 1.5, 100, 0, turn_str);

    // Draw time
    char time_str[10];
    char *time_str_format = "Time: %d";
    int time_to_int = (int)gm->time;
    sprintf(time_str, time_str_format, time_to_int);
    al_draw_text(gm->font, al_map_rgb(255, 255, 255), (SCREEN_WIDTH / 2) * 1.5, 150, 0, time_str);
}

void drawMinigame(GameManager *gm)
{
    al_draw_bitmap(algif_get_bitmap(gm->gifmaps[0], al_get_time()), 1000, 505, 0); // Draws the player, hard coded bad habit
    if (gm->minigame_active)
    {
        al_draw_rotated_bitmap(gm->bitmaps[BOSS_0], 40, 51, gm->boss.Transform.x, gm->boss.Transform.y, gm->boss.Transform.angle, 0); // Draws the boss if visible
        // Draw text 
        char text[100];
        char *text_format = "Skeletron has awoken!";
        sprintf(text, text_format);
        // Text in purple
        al_draw_text(gm->font, al_map_rgb(153, 0, 204), 0, SCREEN_HEIGHT, 0, text);

    }
}

void drawGameOver(GameManager *gm)
{
    // Draw the game over screen
    // Position is hardcoded, but it's just a test
    int score = gm->score;
    char score_str[10];

    char *score_str_format = "Score: %d";
    sprintf(score_str, score_str_format, score);
    al_draw_text(gm->font, al_map_rgb(255, 255, 255), (SCREEN_WIDTH / 2) * 1.5, 0, 0, score_str);

    // Draw Gameover text
    char text[100];
    char *text_format = "You were slain!";
    sprintf(text, text_format);
    // Text in red
    al_draw_text(gm->font, al_map_rgb(204, 0, 0), (SCREEN_WIDTH/2) -100, SCREEN_HEIGHT/2, 0, text);
}

void drawHelp(GameManager *gm)
{
    // Draw the help screen
    // Position is hardcoded, but it's just a test
    int score = gm->score;
    char score_str[10];

    char *score_str_format = "Score: %d";
    sprintf(score_str, score_str_format, score);
    al_draw_text(gm->font, al_map_rgb(255, 255, 255), (SCREEN_WIDTH / 2) * 1.5, 0, 0, score_str);

    // Draw Gameover text
    char text[100];
    char *text_format = "Help!";
    sprintf(text, text_format);
    // Text in red
    al_draw_text(gm->font, al_map_rgb(255, 0, 0), 0, SCREEN_HEIGHT, 0, text);
}

void drawMenu(GameManager* gm){
    // Draw the menu screen, with instructions how to playte

    // Draw the menu screen
    // Position is hardcoded, but it's just a test
    char* text = "Press any key to start";
    char* text2= "Press H for help";
    char* text3= "Press ESC to exit";
    char* text4= "Click on the jewels to swap them! match 3 or more to get points!";

    // Text in white
    al_draw_text(gm->font, al_map_rgb(255, 255, 255), (50), SCREEN_HEIGHT/2, 0, text);
    al_draw_text(gm->font, al_map_rgb(255, 255, 255), (50), SCREEN_HEIGHT/2 + 50, 0, text2);
    al_draw_text(gm->font, al_map_rgb(255, 255, 255), (50), SCREEN_HEIGHT/2 + 100, 0, text3);
    al_draw_text(gm->font, al_map_rgb(255, 255, 255), (50), SCREEN_HEIGHT/2 + 150, 0, text4);
}