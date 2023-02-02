// Autor: B. Fuchs
// Implementation of the game manager

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gameManager.h"

// Brief: This function initializes the game manager
// Param: gameManager *gm: The game manager to initialize
// Return: void
GameManager* initGame(GameManager *game){
    // Initialize the game manager malloc
    game = malloc(sizeof(GameManager));

    // Initialize the game manager
    game->state = 0;
    game->score = 0;

    // 
}