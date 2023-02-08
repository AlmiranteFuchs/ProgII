
#include "gameLogic.h"
#include "gameGraphics.h"

// Prototype
GameManager *InitGameManager();
void _initGameBoard(GameManager *gameManager);
void _initGameBoardPiece(GameManager *gameManager, int i, int j);
void _moveTiles(GameManager *gameManager);
void _swapTiles(GameManager *gameManager, Tile *tile1, Tile *tile2);
void _inputEvent(GameManager *gameManager);
void _checkMatchs(GameManager *gameManager);
int _check_matchs(GameManager *GameManager);
void _destroyTiles(GameManager *gameManager);
void _fallTiles(GameManager *gameManager);

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

int player_play = 0;
// Calls to each time the game frames update
void UpdateGame(GameManager *gameManager)
{
    // Move tiles
    _moveTiles(gameManager);

    // Fall tiles
    _fallTiles(gameManager);
    
    // Check matchs
    _checkMatchs(gameManager);


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
    gm->gameState = GAME_STATE_BLOCK_INPUT;

    // Movement
    tile1->transform.dx = tile2->transform.x;
    tile1->transform.dy = tile2->transform.y;

    tile2->transform.dx = tile1->transform.x;
    tile2->transform.dy = tile1->transform.y;

    tile1->transform.moving = 1;
    tile2->transform.moving = 1;

    // Swap the tiles
    Tile temp = *tile1;
    *tile1 = *tile2;
    *tile2 = temp;

    // Update the selected tiles
    gm->selectedTile = tile2;
    gm->lastSelectedTile = tile1;

    gm->lastSelectedTile->selected = 0;

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

            if (tile->transform.moving)
            {

                // Calc velocity
                int final_velocity = tile->transform.acceleration * tile->transform.moving_t;
                tile->transform.moving_t++;

                // If moving
                if (tile->transform.x != tile->transform.dx || tile->transform.y != tile->transform.dy)
                {
                    // Move the tile
                    if (tile->transform.x < tile->transform.dx)
                    {
                        tile->transform.x += final_velocity;
                    }
                    else if (tile->transform.x > tile->transform.dx)
                    {
                        tile->transform.x -= final_velocity;
                    }

                    if (tile->transform.y < tile->transform.dy)
                    {
                        tile->transform.y += final_velocity;
                    }
                    else if (tile->transform.y > tile->transform.dy)
                    {
                        tile->transform.y -= final_velocity;
                    }

                    // if close enough
                    if (abs(tile->transform.x - tile->transform.dx) < final_velocity)
                    {
                        tile->transform.x = tile->transform.dx;
                    }
                    if (abs(tile->transform.y - tile->transform.dy) < final_velocity)
                    {
                        tile->transform.y = tile->transform.dy;
                    }

                    // Set velocity according to distance
                    int distance = abs(tile->transform.x - tile->transform.dx) + abs(tile->transform.y - tile->transform.dy);
                    final_velocity = (distance / 10) + 1.5;

                    // Change game state to block input
                    gm->gameState = GAME_STATE_BLOCK_INPUT;
                }
                else
                {
                    tile->transform.moving = 0;
                    tile->transform.moving_t = 0;
                    tile->falling = 0;
                    // Change game state to block input
                    gm->gameState = GAME_STATE_GAMEPLAY;
                }
            }
        }
    }
}

// Check for matchs
void _checkMatchs(GameManager *gameManager)
{
    // Check matchs
    if (gameManager->gameState != GAME_STATE_BLOCK_INPUT)
    {
        // If the game is not busy animating something
        // Check matchs
        int matchs = _check_matchs(gameManager);

        if (matchs > 0)
        {
            // If there are matchs
            // Add score
            gameManager->score += matchs * 10;

            // Destroy tiles
            _destroyTiles(gameManager);

            // Reset selected tiles
            gameManager->selectedTile = NULL;
            gameManager->lastSelectedTile = NULL;
            player_play = 0;
        }
        else
        {
            // If there are no matchs
            // Change game state to gameplay
            gameManager->gameState = GAME_STATE_GAMEPLAY;

            // If player played and there are no matchs
            // Change game state to block input and switch tiles
            if (player_play)
            {
                // Switch tiles
                _swapTiles(gameManager, gameManager->selectedTile, gameManager->lastSelectedTile);

                // Reset selected tiles
                gameManager->selectedTile = NULL;
                gameManager->lastSelectedTile = NULL;
            }
            player_play = 0;
        }
    }
}
int _check_matchs(GameManager *GameManager)
{
    int matchs = 0;
    // Check for matchs
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {

            if (GameManager->board[i][j].value == -1 || GameManager->board[i][j].transform.moving)
            {
                // ignore
                continue;
            }

            // Check for horizontal matchs
            if (i < BOARD_WIDTH - 2)
            {

                if (GameManager->board[i][j].value == GameManager->board[i + 1][j].value && GameManager->board[i][j].value == GameManager->board[i + 2][j].value)
                {
                    GameManager->board[i][j].value = -1;
                    GameManager->board[i + 1][j].value = -1;
                    GameManager->board[i + 2][j].value = -1;
                    matchs++;
                }
            }

            // Check for vertical matchs
            if (j < BOARD_HEIGHT - 2)
            {
                if (GameManager->board[i][j].value == GameManager->board[i][j + 1].value && GameManager->board[i][j].value == GameManager->board[i][j + 2].value)
                {
                    GameManager->board[i][j].value = -1;
                    GameManager->board[i][j + 1].value = -1;
                    GameManager->board[i][j + 2].value = -1;
                    matchs++;
                }
            }

            // // Check for L matchs
            // if (i < BOARD_WIDTH - 2 && j < BOARD_HEIGHT - 2)
            // {
            //     if (GameManager->board[i][j].value == GameManager->board[i + 1][j].value && GameManager->board[i][j].value == GameManager->board[i + 2][j + 1].value)
            //     {
            //         GameManager->board[i][j].selected = 1;
            //         GameManager->board[i + 1][j].selected = 1;
            //         GameManager->board[i + 2][j + 1].selected = 1;
            //         matchs++;
            //     }
            // }
        }
    }
    return matchs;
}

void _destroyTiles(GameManager *gameManager)
{
    // Destroy tiles
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {
            if (gameManager->board[i][j].value == -1)
            {
                // If tile is selected
                // Destroy tile
                // gameManager->board[i][j].sprite.path = bitmapPaths[2];
                gameManager->board[i][j].sprite.sprite_num = -1;
                gameManager->board[i][j].value = -1;

                // Debug only
                gameManager->board[i][j].transform.moving = 0;
            }
        }
    }
}

void _fallTiles(GameManager *gameManager)
{
    // Fall tiles
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        // Down for
        for (int j = BOARD_HEIGHT - 1; j > 0; j--)
        {
            Tile *current_tile = &gameManager->board[i][j];
            Tile *above_tile = &gameManager->board[i][j - 1];

            // If current tile is empty
            if (current_tile->value == -1 && current_tile->transform.moving == 0 && above_tile->transform.moving == 0 && above_tile->value != -1)
            {
                // Swap with the above
                Tile *above_tile = &gameManager->board[i][j - 1];

                _swapTiles(gameManager, current_tile, above_tile);
                gameManager->selectedTile = NULL;
                gameManager->lastSelectedTile = NULL;
            }
        }

        if (gameManager->board[i][0].value == -1)
        {
            _initGameBoardPiece(gameManager, i, 0);
        }
    }
}

// Creates a new tile
// Updates the tiles on the game board
void _initGameBoardPiece(GameManager *gameManager, int i, int j)
{
    // Screen scale
    int screenScale = (SCREEN_HEIGHT / BOARD_WIDTH) - 5;

    // Create a new tile
    int tile_type = (rand() % 4) + 1;
    // Fisical Props
    gameManager->board[i][j].transform.x = (i * screenScale) + OFFSET;
    gameManager->board[i][j].transform.y = (j * screenScale) + OFFSET;
    gameManager->board[i][j].transform.velocity = 5;
    gameManager->board[i][j].transform.acceleration = 1.0003;
    gameManager->board[i][j].transform.moving_t = 0;
    gameManager->board[i][j].transform.dx = gameManager->board[i][j].transform.x;
    gameManager->board[i][j].transform.dy = gameManager->board[i][j].transform.y;
    gameManager->board[i][j].transform.moving = 0;

    gameManager->board[i][j].real_posX = i;
    gameManager->board[i][j].real_posY = j;
    gameManager->board[i][j].falling = 0;
    gameManager->board[i][j].value = tile_type;
    gameManager->board[i][j].sprite.sprite_num = tile_type - 1;
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
            _initGameBoardPiece(gameManager, i, j);
        }
    }
}

// // // --- Events --- // // //
void _inputEvent(GameManager *gm)
{
    // Check if the game is in gameplay state
    if (gm->gameState == GAME_STATE_GAMEPLAY)
    {
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
                // Print

                _swapTiles(gm, tile, gm->selectedTile);
                gm->selectedTile->selected = 0;
                player_play = 1;
            }
        }
    }
}