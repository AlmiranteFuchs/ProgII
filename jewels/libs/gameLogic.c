
#include "gameLogic.h"
#include "gameGraphics.h"

// Prototype
GameManager *InitGameManager();
void _initGameBoard(GameManager *gameManager);
void _updateGameBoard(GameManager *gameManager, int i, int j);
void _moveTiles(GameManager *gameManager);
void _inputEvent(GameManager *gameManager);

/**
 * // // // // ---- Public ---- // // // //
 */
// Create a new game object, Init game
GameManager *InitGameManager()
{
    GameManager *gameManager = (GameManager *)malloc(sizeof(GameManager));
    gameManager->score = 0;
    gameManager->gameState = GAME_STATE_GAMEPLAY; // TODO: Menu
    _initGameBoard(gameManager);
    return gameManager;
}

// Call to draw the game
void DrawGame(GameManager *gm)
{
    // Calls to graphics functions
    drawBackground();
    drawTiles(gm->board);
    drawUI();
}

// Calls to each time the game frames update
void UpdateGame(GameManager *gameManager)
{
    // Move tiles
    _moveTiles(gameManager);

    // Switch game events
    switch (gameManager->gameEvent)
    {
    case GAME_EVENT_INPUT:
        _inputEvent(gameManager);
        gameManager->gameEvent = GAME_EVENT_NONE;
        break;
    default:
        break;
    }
}
/*
 * // // // // ---- Private ---- // // // //
 */
// // // --- Input --- // // //
// Check click on tile by mouse x y
Tile *_checkClickOnTile(GameManager *gameManager, int x, int y)
{
    // Screen scale
    int screenScale = (SCREEN_HEIGHT / BOARD_WIDTH) - 5;

    // Check if click is on the board
    if (x > BOARD_WIDTH * screenScale || y > BOARD_HEIGHT * screenScale)
    {
        return NULL;
    }

    // The offset here is the offset of the board
    // Get the tile
    int i = (x - OFFSET) / screenScale;
    int j = (y - OFFSET) / screenScale;

    Tile *tile = &gameManager->board[i][j];
    printf("Clicked on tile: %d %d, name of sprite: %s\n", i, j, tile->sprite.path);

    return &gameManager->board[i][j];
}

// // // --- Logic --- // // //
// Swap Tiles
void _swapTiles(GameManager *gm, Tile *tile1, Tile *tile2)
{
    // Swap the values
    // int temp = tile1->value;
    // tile1->value = tile2->value;
    // tile2->value = temp;

    // Change game state to block input
    // gm->gameState = GAME_STATE_BLOCK_INPUT;

    // Movement
    tile1->transform.dx = tile2->transform.x;
    tile1->transform.dy = tile2->transform.y;

    tile2->transform.dx = tile1->transform.x;
    tile2->transform.dy = tile1->transform.y;


    // Swap pos on matrix
    int tempX = tile1->real_posX;
    int tempY = tile1->real_posY;

    tile1->real_posX = tile2->real_posX;
    tile1->real_posY = tile2->real_posY;
    
    tile2->real_posX = tempX;
    tile2->real_posY = tempY;
        return;
}

// Moves the tiles
void _moveTiles(GameManager *gm)
{
    // For all tiles in the board
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {
            // Selected tile
            Tile *tile = &gm->board[i][j];

            // If moving
            if (tile->transform.x != tile->transform.dx || tile->transform.y != tile->transform.dy)
            {
                printf("Moving tile: %d %d\n", i, j);
                // Move the tile
                if (tile->transform.x < tile->transform.dx)
                {
                    tile->transform.x += 1;
                }
                else if (tile->transform.x > tile->transform.dx)
                {
                    tile->transform.x -= 1;
                }

                if (tile->transform.y < tile->transform.dy)
                {
                    tile->transform.y += 1;
                }
                else if (tile->transform.y > tile->transform.dy)
                {
                    tile->transform.y -= 1;
                }

                // // If the tile is in the destination
                // if (tile->transform.x == tile->transform.dx && tile->transform.y == tile->transform.dy)
                // {
                //     // Change game state to allow input
                //     gm->gameState = GAME_STATE_GAMEPLAY;
                // }
                // else
                // {
                //     // Change game state to block input
                //     // gm->gameState = GAME_STATE_BLOCK_INPUT;
                // }

                // Change game state to block input
                // gm->gameState = GAME_STATE_BLOCK_INPUT;
            }
            else
            {
                // If not moving
                // Change game state to allow input
                gm->gameState = GAME_STATE_GAMEPLAY;
            }
        }
    }
}

// Creates a new tile
// Updates the tiles on the game board
void _updateGameBoard(GameManager *gameManager, int i, int j)
{
    // Screen scale
    int screenScale = (SCREEN_HEIGHT / BOARD_WIDTH) - 5;

    // Create a new tile
    int tile_type = (rand() % 5) + 1;
    gameManager->board[i][j].transform.x = (i * screenScale) + OFFSET;
    gameManager->board[i][j].transform.y = (j * screenScale) + OFFSET;
    gameManager->board[i][j].transform.velocity = 0.001;
    gameManager->board[i][j].transform.dx = gameManager->board[i][j].transform.x;
    gameManager->board[i][j].transform.dy = gameManager->board[i][j].transform.y;
    gameManager->board[i][j].real_posX = i;
    gameManager->board[i][j].real_posY = j;
    gameManager->board[i][j].value = tile_type;
    gameManager->board[i][j].sprite.path = bitmapPaths[tile_type];
    gameManager->board[i][j].sprite.height = 40;
    gameManager->board[i][j].sprite.width = 40;
    gameManager->board[i][j].selected = 0;
    // TODO: Implement this later
    // if(j == 0)
    // {
    //     gameManager->board[i][j].transform.visible = 0;
    //     return;
    // }
    gameManager->board[i][j].transform.visible = 1;
}

// Initialize game board
void _initGameBoard(GameManager *gameManager)
{
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {
            _updateGameBoard(gameManager, i, j);
        }
    }
}

// // // --- Events --- // // //
void _inputEvent(GameManager *gm)
{
    printf("Game state= %d\n", gm->gameState);
    // Check if the game is in gameplay state
    if (gm->gameState == GAME_STATE_GAMEPLAY)
    {
        printf("Gaming");
        printf("Game state: %d\n", gm->gameState);
        int x = gm->mouseX;
        int y = gm->mouseY;

        // Mouse is clicked
        // Check if click is on a tile
        Tile *tile = _checkClickOnTile(gm, x, y);
        if (tile != NULL)
        {
            // If there is no selected tile
            if (gm->selectedTile == NULL)
            {
                // Select the tile
                gm->selectedTile = tile;
                tile->selected = 1;
            }
            else
            {
                // If there is a selected tile
                // Swap the tiles
                _swapTiles(gm, tile, gm->selectedTile);
                // Deselect the tile
                gm->selectedTile->selected = 0;
                gm->selectedTile = NULL;
            }
        }
    }
}