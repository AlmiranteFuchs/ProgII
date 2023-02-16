// Author: B. Fuchs
#include "gameGraphics.h"
#include "gameObjects.h"
#include "gameAudio.h"

#ifndef GAMELOGIC_H
#define GAMELOGIC_H

// ???????????
// Fuck me

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 576

// Tile sprite types
#define OFFSET 35
#define OFFSET_Y 60

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 9 // 9 bc one of them is for new line generating

#define FOREGROUNDS 3
#define CENARY_OBJS 5
#define BITMAPS_COUNT 14
#define GIFMAPS_COUNT 3
#define AUDIO_COUNT 3

#include <stdio.h>

#include "allegro5/allegro5.h"
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "algif.h"

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
    float time;
    int turn;
    int level;

    GameState gameState; // Game state
    GameEvent gameEvent; // Game event

    // Event driven
    int mouseX;
    int mouseY;
    int key;

    // Game objects
    Tile board[BOARD_WIDTH][BOARD_HEIGHT];
    Tile *selectedTile;
    Tile *lastSelectedTile;
    AudioPlayer audio;

    // Simple Cenary objects
    SimpleObject background;
    SimpleObject foregrounds[FOREGROUNDS];
    SimpleObject cenaryObjects[CENARY_OBJS];
    Boss boss;

    // Minigame
    int minigame_active;
    int minigame_time;
    // Minigame activation sequence
    int minigame_sequence[3];

    // Allegro objects
    ALLEGRO_EVENT event;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_DISPLAY *disp;
    ALLEGRO_FONT *font;
    ALLEGRO_BITMAP *bitmaps[BITMAPS_COUNT];
    ALGIF_ANIMATION *gifmaps[GIFMAPS_COUNT];
    ALLEGRO_SAMPLE *audioSamples[AUDIO_COUNT];
    
} GameManager;

// // // -- Public -- // // //

// Initialize game
GameManager *InitGameManager();

// Destroy game
void DestroyGameManager(GameManager *gameManager);

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