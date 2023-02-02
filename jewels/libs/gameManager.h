// Autor: B. Fuchs
// Info: Header file for game objects of the jewels game

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <stdlib.h>

// Game board
#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

// Game states
#define GAME_OVER 0
#define GAME_RUNNING 1
#define GAME_WON 2


// Game manager
typedef struct {
    int board[BOARD_WIDTH][BOARD_HEIGHT];
    int score;
    int state;
} GameManager;

// Structs for game objects
// The idea is to have a component based game object system

// Object transform
typedef struct {
    int x;
    int y;
    int dx;
    int dy;
    int angle;
    int angleSpeed;
    int visible;
} Transform;

// Object sprite
typedef struct {
    int width;
    int height;
    int color;
    char* path;
} Sprite;

// Object collider
// typedef struct {
//     int width;
//     int height;
//     int x;
//     int y;
// } Collider;



// Initialize game
GameManager* initGame(GameManager *game);

// Check if game is over
int isGameOver(GameManager *game);

// Check if game is won
int isGameWon(GameManager *game);

// Check if game is running
int isGameRunning(GameManager *game);

#endif // GAMEMANAGER_H