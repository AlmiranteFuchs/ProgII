// Brief: This file contains the main functions for the jewels game
// Author: Bruno Fuchs
// Date: 13/01/2023
// Version: 1.0

// Default libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "libs/gameLogic.h"


void Run_game(GameManager *gm)
{
    bool done = false;
    bool redraw = true;

    while (1)
    {
        al_wait_for_event(gm->queue, &gm->event);

        switch (gm->event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            // game logic goes here.
            UpdateGame(gm);

            redraw = true;
            break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            gm->gameEvent = GAME_EVENT_INPUT;
            gm->mouseX = gm->event.mouse.x;
            gm->mouseY = gm->event.mouse.y;

            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            switch (gm->event.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            // Case any other
            default:
                gm->key = gm->event.keyboard.keycode;
                printf("Key pressed: %d\n", gm->key);
                break;
            }
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        if (redraw && al_is_event_queue_empty(gm->queue))
        {
            DrawGame(gm);
            al_flip_display();

            redraw = false;
        }
    }

    // Exit the game
    DestroyGameManager(gm);
}

int main(int argc, char *argv[])
{
    printf("Initializing!\n");
    // Audio
    // Create a new game object
    GameManager *gm = InitGameManager();
    printf("Initialized!\n");

    Run_game(gm);
    return 0;
}
