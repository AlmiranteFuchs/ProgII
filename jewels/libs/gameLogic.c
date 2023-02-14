
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
void _initTransformDefault(Transform *transform);
void _moveForeground(GameManager *gm);

/**
 * // // // // ---- Public ---- // // // //
 */
// Create a new game object, Init game
GameManager *InitGameManager()
{

    GameManager *gameManager = (GameManager *)malloc(sizeof(GameManager));
    gameManager->score = 0;
    gameManager->turn = 0;
    gameManager->gameState = GAME_STATE_GAMEPLAY; // TODO: Menu
    gameManager->gameEvent = GAME_EVENT_NONE;

    // Init audio
    gameManager->audio.audio_num = AUDIO_MUSIC;
    gameManager->audio.volume = 0.3f;
    gameManager->audio.speed = 1.0f;
    gameManager->audio.loop = 1;
    gameManager->audio.playing = 1;
    playAudio(&gameManager->audio);

    // Init game board
    _initGameBoard(gameManager);

    // Init background
    gameManager->background.Transform.x = 0;
    gameManager->background.Transform.y = 0;
    gameManager->background.Transform.dy = 0;
    gameManager->background.Transform.dx = 0;
    gameManager->background.Transform.visible = 1;
    gameManager->background.sprite.sprite_num = UI_BACKGROUND_12;
    gameManager->background.sprite.width = 1920;
    gameManager->background.sprite.height = 1080;

    // Init foreground
    int offset_y = 240;
    for (int i = 0; i < 3; i++)
    {
        _initTransformDefault(&gameManager->foregrounds[i].Transform);
        gameManager->foregrounds[i].Transform.x = 1026 * i;
        gameManager->foregrounds[i].Transform.y = offset_y;
    }

    return gameManager;
}

void _initTransformDefault(Transform *transform)
{
    transform->x = 0;
    transform->y = 0;
    transform->dx = 0;
    transform->dy = 0;
    transform->acceleration = 1;
    transform->angle = 0;
    transform->angleSpeed = 0;
    transform->moving = 0;
    transform->moving_t = 0;
    transform->velocity = 1;
    transform->visible = 1;
}

// Call to draw the game
void DrawGame(GameManager *gm)
{
    // Calls to graphics functions
    drawBackground(gm);
    drawTiles(gm);
    drawUI(gm);
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

    // Move foreground
    _moveForeground(gameManager);

    // Update game time, game runs in 60 fps
    gameManager->time = gameManager->time + (1 / 60.0f);
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
    int j = (y - (OFFSET - OFFSET_Y)) / screenScale;

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

    int temp_x1 = tile1->real_posX;
    int temp_y1 = tile1->real_posY;

    tile1->real_posX = tile2->real_posX;
    tile2->real_posX = temp_x1;

    tile1->real_posY = tile2->real_posY;
    tile2->real_posY = temp_y1;

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
                // Possible Tile Matches
                int current_tile = GameManager->board[i][j].value;
                int next_tile = GameManager->board[i + 1][j].value;
                int next_next_tile = GameManager->board[i + 2][j].value;
                int next_next_next_tile = GameManager->board[i + 3][j].value;
                int next_next_next_next_tile = GameManager->board[i + 4][j].value;

                if (current_tile == next_tile && current_tile == next_next_tile)
                {
                    // Play sound
                    playAudio(&GameManager->board[i][j].audioPlayer);

                    // Destroy Tiles
                    GameManager->board[i][j].value = -1;
                    GameManager->board[i + 1][j].value = -1;
                    GameManager->board[i + 2][j].value = -1;

                    // If there is a 4th match
                    if (next_next_tile == next_next_next_tile)
                    {
                        // New special jewel
                        GameManager->board[i + 2][j].value = 6;
                        GameManager->board[i + 2][j].sprite.sprite_num = 6;

                        // Clear 4th
                        GameManager->board[i + 3][j].value = -1;
                        GameManager->board[i + 3][j].sprite.sprite_num = -1;

                        // If there is a 5th match
                        if (next_next_next_next_tile == current_tile)
                        {
                            // New special jewel
                            GameManager->board[i + 2][j].value = 6;
                            GameManager->board[i + 2][j].sprite.sprite_num = 6;

                            // Clear 5th
                            GameManager->board[i + 4][j].value = -1;
                            GameManager->board[i + 4][j].sprite.sprite_num = -1;

                            matchs++;
                        }

                        matchs++;
                    }

                    matchs++;
                    // Calculate score
                    int score = (matchs * 20);

                    // If matchs are special type
                    if (current_tile == 6)
                    {
                        score = score + (100 * matchs);
                    }

                    printf("Score: %d\n", GameManager->score);
                    GameManager->score += score;
                }
            }

            // Check for vertical matchs
            if (j < BOARD_HEIGHT - 2)
            {
                // Possible Tile Matches
                int current_tile = GameManager->board[i][j].value;
                int next_tile = GameManager->board[i][j + 1].value;
                int next_next_tile = GameManager->board[i][j + 2].value;
                int next_next_next_tile = GameManager->board[i][j + 3].value;
                int next_next_next_next_tile = GameManager->board[i][j + 4].value;

                if (current_tile == next_tile && current_tile == next_next_tile)
                {
                    // Play sound
                    playAudio(&GameManager->board[i][j].audioPlayer);

                    // Destroy Tiles
                    GameManager->board[i][j].value = -1;
                    GameManager->board[i][j + 1].value = -1;
                    GameManager->board[i][j + 2].value = -1;

                    // If there is a 4th match
                    if (next_next_tile == next_next_next_tile)
                    {
                        // New special jewel
                        GameManager->board[i][j + 2].value = 6;
                        GameManager->board[i][j + 2].sprite.sprite_num = 6;

                        // Clear 4th
                        GameManager->board[i][j + 3].value = -1;
                        GameManager->board[i][j + 3].sprite.sprite_num = -1;

                        // If there is a 5th match
                        if (next_next_next_next_tile == current_tile)
                        {
                            // New special jewel
                            GameManager->board[i][j + 2].value = 6;
                            GameManager->board[i][j + 2].sprite.sprite_num = 6;

                            // Clear 5th
                            GameManager->board[i][j + 4].value = -1;
                            GameManager->board[i][j + 4].sprite.sprite_num = -1;

                            matchs++;
                        }

                        matchs++;
                    }

                    matchs++;
                    // Calculate score
                    int score = (matchs * 20);

                    // If matchs are special type
                    if (current_tile == 6)
                    {
                        score = score + (100 * matchs);
                    }
                    printf("Score: %d\n", GameManager->score);
                    GameManager->score += score;
                }
            }
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

                current_tile->transform.moving_t += 5;
                above_tile->transform.moving_t += 5;

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


// Moves the foreground in loop
void _moveForeground(GameManager *gm)
{
    // Keep moving the 3 foregrounds
    for (int i = 0; i < 3; i++)
    {
        gm->foregrounds[i].Transform.x -= 1;

        // If the foreground is out of the screen
        if (gm->foregrounds[i].Transform.x < -SCREEN_WIDTH)
        {
            // Move it to the right
            gm->foregrounds[i].Transform.x = SCREEN_WIDTH;
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
    int tile_type = (rand() % 5) + 1;
    // Fisical Props
    gameManager->board[i][j].transform.x = (i * screenScale) + OFFSET;
    gameManager->board[i][j].transform.y = (j * screenScale) + OFFSET - OFFSET_Y;
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

    // Sound
    gameManager->board[i][j].audioPlayer.audio_num = 1;
    gameManager->board[i][j].audioPlayer.loop = 0;
    gameManager->board[i][j].audioPlayer.volume = 1.0f;
    gameManager->board[i][j].audioPlayer.playing = 1;
    gameManager->board[i][j].audioPlayer.speed = 1.0f;

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

                // Check distance between points
                int distance_x = abs(tile->real_posX - gm->selectedTile->real_posX);
                int distance_y = abs(tile->real_posY - gm->selectedTile->real_posY);

                if (!(abs(distance_x + distance_y) > 1))
                {
                    _swapTiles(gm, tile, gm->selectedTile);
                    gm->selectedTile->selected = 0;
                    player_play = 1;
                    gm->turn++;

                    printf("Turn: %d\n", gm->turn);
                }
            }
        }
    }
}