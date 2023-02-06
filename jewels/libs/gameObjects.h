// Autor: B. Fuchs
// Game objects header file

#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include "gameComponents.h"

// Game objects
// Jewels tile

// Window size
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Game board
#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

// Game states
#define GAME_OVER 0
#define GAME_RUNNING 1
#define GAME_WON 2

// Tile sprite types
#define TILE_SPRITES 5

// Represents a single tile, a jewel
typedef struct
{
    Transform transform;
    Sprite sprite;
    int value;
} Tile;

// Game manager Object, game state and board
typedef struct
{
    Tile board[BOARD_WIDTH][BOARD_HEIGHT];
    int score;
    int state;
} GameManager;

// Initialize game manager
GameManager *InitGameManager();

// Initialize game board
void InitGameBoard(GameManager *gameManager);

// Print game board
void PrintGameBoard(GameManager *gameManager);


#endif // GAMEOBJECTS_H