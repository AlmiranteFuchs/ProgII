
#include "gameGraphics.h"
#include <stdio.h>

ALLEGRO_COLOR mapValueToColor(int value);

void drawTile(Tile tile)
{
    printf("Drawing tile at %d, %d\n", tile.transform.x, tile.transform.y);
    al_draw_filled_rectangle(tile.transform.x, tile.transform.y, tile.transform.x + TILE_SIZE, tile.transform.y + TILE_SIZE, mapValueToColor(tile.value));
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

// Map value to color
ALLEGRO_COLOR mapValueToColor(int value)
{
    switch (value)
    {
    case 1:
        return al_map_rgb(255, 0, 0);
    case 2:
        return al_map_rgb(0, 255, 0);
    case 3:
        return al_map_rgb(0, 0, 255);
    case 4:
        return al_map_rgb(255, 255, 0);
    case 5:
        return al_map_rgb(255, 0, 255);
    case 6:
        return al_map_rgb(0, 255, 255);
    default:
        return al_map_rgb(0, 0, 0);
    }
}