// Author: B. Fuchs

#ifndef GAME_GRAPHICS_H
#define GAME_GRAPHICS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "gameObjects.h"
#include "gameLogic.h" // Just for the board size


// Draw the tiles, receives a matrix of tiles
void drawTiles(Tile tiles[BOARD_WIDTH][BOARD_HEIGHT]);
void drawTile(Tile tile);

// Load the bitmaps
void loadBitmaps();

// Draw UI
void drawUI(GameManager *gm);

// Draw background 
void drawBackground();


#endif // GAME_GRAPHICS_H