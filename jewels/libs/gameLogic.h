// Author: B. Fuchs

#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 576

// Tile sprite types
#define OFFSET 35
#define OFFSET_Y 60 

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 9 // 9 bc one of them is for new line generating

#include "gameObjects.h"
#include <stdio.h>

// Enum game events
typedef enum
{
    GAME_EVENT_NONE,
    GAME_EVENT_INPUT,  // Logic on input
    GAME_EVENT_UPDATE, // For movement

} GameEvent;

// Enum game states
typedef enum
{
    GAME_STATE_NONE,
    GAME_STATE_TITLE,
    GAME_STATE_MENU,
    GAME_STATE_GAMEPLAY,
    GAME_STATE_BLOCK_INPUT,
    GAME_STATE_GAMEOVER,
    GAME_STATE_EXIT,
} GameState;


// Game manager Object, game state and board
typedef struct
{
    // Game properties
    int score;
    int time;
    int turn;
    int level;

    GameState gameState; // Game state
    GameEvent gameEvent; // Game event

    // Event driven
    int mouseX;
    int mouseY;

    // Game objects
    Tile board[BOARD_WIDTH][BOARD_HEIGHT];
    Tile *selectedTile;
    Tile *lastSelectedTile;

} GameManager;

// // // -- Public -- // // //

// Initialize game
GameManager *InitGameManager();

// Update game
void UpdateGame(GameManager *gameManager);

// Draw game
void DrawGame(GameManager *gameManager);

// Exit game
void ExitGame(GameManager *gameManager);

// Logic events on game states
void UpdateGameplay(GameManager *gameManager);
void UpdateGameOver(GameManager *gameManager);

// Logic events on game events
void DrawGameplay(GameManager *gameManager);
void DrawGameOver(GameManager *gameManager);

#endif // GAMELOGIC_H