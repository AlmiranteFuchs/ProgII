
#include "gameLogic.h"

// Allegro
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

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
void _manageCenary(GameManager *gm);
void _manageMinigame(GameManager *gm);
void _checkMinigameActivation(GameManager *gm);

/**
 * // // // // ---- Private Initializes ---- // // // //
 */

// Initializes vital code
void _must_init(bool test, const char *description)
{
    if (test)
        return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

// Initializes a transform with default values
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

/**
 * // // // // ---- Public ---- // // // //
 */
// Create a new game object, Init game
GameManager *InitGameManager()
{

    GameManager *gm = (GameManager *)malloc(sizeof(GameManager));

    // Init Allegro stuff
    _must_init(al_init(), "allegro");
    _must_init(al_install_keyboard(), "keyboard");
    _must_init(al_install_mouse(), "mouse");
    _must_init(al_install_audio(), "audio");
    _must_init(al_init_image_addon(), "image addon");
    _must_init(al_init_primitives_addon(), "primitives");
    _must_init(al_init_font_addon(), "font addon");
    _must_init(al_init_ttf_addon(), "ttf addon");
    _must_init(al_init_acodec_addon(), "acodec addon");

    _must_init((gm->timer = al_create_timer(1.0 / 60.0)), "timer");
    _must_init((gm->queue = al_create_event_queue()), "queue");
    _must_init((gm->disp = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT + 35)), "display");

    // ??
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    al_set_new_window_title("Terraria Crush");
    al_reserve_samples(16);

    // Register the events
    al_register_event_source(gm->queue, al_get_keyboard_event_source());
    al_register_event_source(gm->queue, al_get_display_event_source(gm->disp));
    al_register_event_source(gm->queue, al_get_timer_event_source(gm->timer));
    al_register_event_source(gm->queue, al_get_mouse_event_source());

    al_start_timer(gm->timer);

    printf("Most of allegro stuff loaded\n");

    // Loads the game graphics
    initGraphics(gm);
    printf("Graphics loaded\n");

    // Initializes the game sounds
    initAudio(gm);
    printf("Audio loaded\n");

    gm->score = 0;
    gm->turn = 0;
    gm->gameState = GAME_STATE_GAMEPLAY; // TODO: Menu
    gm->gameEvent = GAME_EVENT_NONE;
    gm->minigame_active = 0;
    // Minigame sequence
    gm->minigame_sequence[0] = 82;
    gm->minigame_sequence[1] = 83;
    gm->minigame_sequence[2] = 84;

    // Init music
    // FIXME: This is a temporary solution, we need to create a sound manager
    gm->audio.audio_num = AUDIO_MUSIC;
    gm->audio.volume = 0.3f;
    gm->audio.speed = 1.0f;
    gm->audio.loop = 1;
    gm->audio.playing = 1;
    playAudio(gm, &gm->audio);

    // Init game board
    _initGameBoard(gm);
    printf("Game board initialized\n");

    // Init background
    _initTransformDefault(&gm->background.Transform);
    gm->background.sprite.width = 1920;  // Hard coded for now
    gm->background.sprite.height = 1080; // Hard coded for now
    gm->background.sprite.sprite_num = UI_BACKGROUND_0;

    // Init foreground
    for (int i = 0; i < FOREGROUNDS; i++)
    {
        _initTransformDefault(&gm->foregrounds[i].Transform);
        gm->foregrounds[i].Transform.x = 1026 * i;
        gm->foregrounds[i].Transform.y = 240;

        gm->foregrounds[i].active = 1;
        gm->foregrounds[i].direction = 0;
        gm->foregrounds[i].cenary_type = CENARY_FOREGROUND;

        gm->foregrounds[i].sprite.height = 1026; // Hard coded for now
        gm->foregrounds[i].sprite.width = 376;   // Hard coded for now
        gm->foregrounds[i].sprite.sprite_num = UI_FOREGROUND_0;
    }

    // Init cenary objects
    for (int i = 0; i < CENARY_OBJS; i++)
    {
        _initTransformDefault(&gm->cenaryObjects[i].Transform);
        gm->cenaryObjects[i].Transform.visible = 0;

        gm->cenaryObjects[i].active = 0;
        gm->cenaryObjects[i].cenary_type = CENARY_OBJECTS;
    }

    _initTransformDefault(&gm->boss.Transform);
    gm->boss.Transform.visible = 0;
    gm->boss.Transform.x = 300;
    gm->boss.Transform.y = 300;

    gm->boss.internal_count = 300;
    gm->boss.sprite.sprite_num = BOSS_0;

    gm->boss.audio.audio_num = AUDIO_BOSS;
    gm->boss.audio.loop = 0;
    gm->boss.audio.playing = 1;
    gm->boss.audio.speed = 1.0f;
    gm->boss.audio.volume = 1.0f;

    gm->minigame_time = 0;

    printf("Done initializing game manager\n");

    return gm;
}

// Destroy everything
void DestroyGameManager(GameManager *gm)
{
    al_destroy_font(gm->font);
    al_destroy_display(gm->disp);
    al_destroy_timer(gm->timer);
    al_destroy_event_queue(gm->queue);
    destroyAudio(gm);
    destroyGraphics(gm);
    free(gm);
}

// Call to draw the game
void DrawGame(GameManager *gm)
{
    // Calls to graphics functions
    drawBackground(gm);
    drawTiles(gm);
    drawUI(gm);

    drawMinigame(gm);
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

    // Manage the cenary
    _manageCenary(gameManager);

    // Manage the minigame
    _manageMinigame(gameManager);

    // Check minigame activation
    _checkMinigameActivation(gameManager);

    // Update game time, game runs in 60 fps
    gameManager->time = gameManager->time + (1 / 60.0f);
}
/*
 * // // // // ---- Private Logic ---- // // // //
 */

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
                    playAudio(GameManager, &GameManager->board[i][j].audioPlayer);

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
                    playAudio(GameManager, &GameManager->board[i][j].audioPlayer);

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
    for (int i = 0; i < FOREGROUNDS; i++)
    {
        SimpleObject *foreground = &gm->foregrounds[i];
        foreground->Transform.x -= foreground->Transform.velocity * (gm->minigame_active ? 2 : 1);

        // If the foreground is out of the screen
        if (foreground->Transform.x < -SCREEN_WIDTH)
        {
            // Move it to the right
            foreground->Transform.x = SCREEN_WIDTH;
        }
    }
}

// Manage elements in the cenary
void _manageCenary(GameManager *gm)
{
    // Move the foreground
    _moveForeground(gm);

    for (int i = 0; i < CENARY_OBJS; i++)
    {
        SimpleObject *object = &gm->cenaryObjects[i];

        // If the object is not active
        if (!object->active)
        {

            // 0.10 chance to spawn, clouds
            if (rand() % 1000000 < 500)
            {
                // Choose a direction, 0 left 1 right
                object->direction = (rand() % 2) ? 1 : -1;

                // Decide type of object to spawn
                object->cenary_type = CENARY_CLOUDS;

                // Spawn the object
                object->active = 1;
                object->Transform.visible = 1;

                // Set y of object
                object->Transform.y = (rand() % SCREEN_HEIGHT) - 250;

                // Set Sprite
                object->sprite.sprite_num = C_CLOUD_0;
            }
            // 0.01 chance to spawn, rabbits
            else if (rand() % 10000 < 100)
            {
                // Choose a direction, 0 left 1 right
                object->direction = (rand() % 2) ? 1 : -1;

                // Spawn as object
                object->cenary_type = CENARY_OBJECTS;

                // Spawn the object
                object->active = 1;
                object->Transform.visible = 1;

                // Set y of object
                object->Transform.y = 525;

                // Set Sprite
                object->sprite.sprite_num = (object->direction == 1 ? 2 : 1);
            }

            // An object was spawned
            if (object->active)
            {
                // Move
                if (object->direction == 1)
                {
                    object->Transform.velocity = 1;
                    object->Transform.x = -300;
                }
                else
                {
                    object->Transform.velocity = gm->minigame_active ? -3 : -2;
                    object->Transform.x = SCREEN_WIDTH;
                }
            }
        }
        else
        {
            // Move the object based on direction
            object->Transform.x += object->Transform.velocity;
            // If the object is out of the screen
            if (object->Transform.x > SCREEN_WIDTH || object->Transform.x < -300)
            {
                // Deactivate the object
                object->active = 0;
                object->Transform.visible = 0;
            }
        }
    }
}

int local_score_minigame = 0;
void _manageMinigame(GameManager *gm)
{
    if (!gm->minigame_active)
        return;

    gm->boss.Transform.visible = 1;

    int player_x = 1000;
    int player_y = 505;

    // Rotate boss
    gm->boss.Transform.angle += 0.15;

    // Get position of boss

    gm->boss.Transform.dx = player_x;
    gm->boss.Transform.dy = player_y;

    gm->minigame_time++;

    // Check if hit by diference bewteen score and local score
    if (gm->score - local_score_minigame > 0)
    {
        // Reset time
        gm->minigame_time = 0;
        // Reset local score
        local_score_minigame = gm->score;
    }

    // Move boss
    int final_velocity;
    // If there's more than 3 seconds wihout a hit
    if (gm->minigame_time > 55)
    {
        // Move the boss
        final_velocity = 1;

        // If the boss is close enough to the player
    }
    else
    {
        // Move the boss
        final_velocity = -1;
    }

    // If moving
    if (gm->boss.Transform.x != gm->boss.Transform.dx || gm->boss.Transform.y != gm->boss.Transform.dy)
    {
        // Move the gm->boss
        if (gm->boss.Transform.x < gm->boss.Transform.dx)
        {
            gm->boss.Transform.x += final_velocity;
        }
        else if (gm->boss.Transform.x > gm->boss.Transform.dx)
        {
            gm->boss.Transform.x -= final_velocity;
        }

        if (gm->boss.Transform.y < gm->boss.Transform.dy)
        {
            gm->boss.Transform.y += final_velocity;
        }
        else if (gm->boss.Transform.y > gm->boss.Transform.dy)
        {
            gm->boss.Transform.y -= final_velocity;
        }

        // if close enough
        if (abs(gm->boss.Transform.x - gm->boss.Transform.dx) < final_velocity)
        {
            gm->boss.Transform.x = gm->boss.Transform.dx;
        }
        if (abs(gm->boss.Transform.y - gm->boss.Transform.dy) < final_velocity)
        {
            gm->boss.Transform.y = gm->boss.Transform.dy;
        }
    }

    // If boss out of screen
    if (gm->boss.Transform.x < 0 || gm->boss.Transform.x > SCREEN_WIDTH || gm->boss.Transform.y < 0 || gm->boss.Transform.y > SCREEN_HEIGHT)
    {
        // Reset minigame
        gm->minigame_active = 0;
        gm->boss.Transform.visible = 0;
        gm->minigame_time = 0;
        local_score_minigame = 0;
    }
}

int count = 0;
int last_pressed = 0;
void _checkMinigameActivation(GameManager *gm)
{
    // Get key pressed by user, check against the key sequence
    if (gm->minigame_active)
        return;

    // If the key pressed is the same as the key sequence
    int pressed_key_code = gm->key;

    if (pressed_key_code == 0 || pressed_key_code == last_pressed)
        return;

    last_pressed = pressed_key_code;

    // If the key pressed is the same as the key sequence
    if (pressed_key_code == gm->minigame_sequence[count])
    {
        // If the key pressed is the last key of the sequence
        if (count == 2)
        {
            // Activate minigame
            gm->minigame_active = 1;
            gm->boss.Transform.visible = 1;
            gm->boss.Transform.x = 200;
            gm->boss.Transform.y = 200;
            count = 0;
            last_pressed = 0;
            printf("Minigame activated\n");

            playAudio(gm, &gm->boss.audio);
        }
        else
        {
            printf("Correct key\n");
            // Increment count
            count++;
        }
    }
    else
    {
        // Reset count
        printf("Wrong key\n");
        count = 0;
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
    gameManager->board[i][j].transform.visible = 1;

    gameManager->board[i][j].real_posX = i; // This is the real position of the tile, used for the board, not really necessary but it helps
    gameManager->board[i][j].real_posY = j; // This is the real position of the tile, used for the board, not really necessary but it helps
    gameManager->board[i][j].falling = 0;
    gameManager->board[i][j].value = tile_type;
    gameManager->board[i][j].sprite.sprite_num = tile_type - 1;
    gameManager->board[i][j].sprite.height = 40;
    gameManager->board[i][j].sprite.width = 40;
    gameManager->board[i][j].selected = 0;

    gameManager->board[i][j].audioPlayer.audio_num = AUDIO_MATCH;
    gameManager->board[i][j].audioPlayer.volume = 1.0f;
    gameManager->board[i][j].audioPlayer.playing = 1;
    gameManager->board[i][j].audioPlayer.loop = 0;
    gameManager->board[i][j].audioPlayer.speed = 1.0f;
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