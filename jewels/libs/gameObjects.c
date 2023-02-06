// Author: B. Fuchs

#include "gameObjects.h"
#include <stdio.h>

// Initialize game board
void InitGameBoard(GameManager *gameManager)
{
    // Screen scale, full screen is 640x480
    int screenScale = (SCREEN_HEIGHT / BOARD_WIDTH);


    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {
            gameManager->board[i][j].transform.x = i * screenScale;
            gameManager->board[i][j].transform.y = j * screenScale;
            gameManager->board[i][j].value = (rand() % 5) +1;
        }
    }
}

// Print game board
void PrintGameBoard(GameManager *gameManager)
{
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {
            printf("%d ", gameManager->board[i][j].value);
        }
        printf("\n");
    }
}

// Create a new game object
GameManager *InitGameManager()
{
    GameManager *gameManager = (GameManager *)malloc(sizeof(GameManager));
    gameManager->score = 0;
    gameManager->state = GAME_RUNNING;
    InitGameBoard(gameManager);
    return gameManager;
}