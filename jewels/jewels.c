// Brief: This file contains the main functions for the jewels game
// Author: Bruno Fuchs
// Date: 13/01/2023
// Version: 1.0

// Default libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "libs/gameObjects.h"
#include "libs/gameGraphics.h"
#include "libs/gameLogic.h"

// Allegro
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

// Last selected tile
Tile *lastSelectedTile = NULL;

void must_init(bool test, const char *description)
{
    if (test)
        return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

void test_function_alegro(GameManager *gm)
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "Mouse");
    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_init_font_addon(), "font addon");
    must_init(al_init_ttf_addon(), "ttf addon");


    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    // Change the display title on the top of the window
    al_set_new_window_title("Terraria Crush");

    ALLEGRO_DISPLAY *disp = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT + 35);
    must_init(disp, "display");

    // Images TODO: Migrate this to gameGraphics
    must_init(al_init_image_addon(), "image addon");
    initGraphics(gm);


    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);

    // Game variables

    while (1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            // game logic goes here.
            UpdateGame(gm);

            redraw = true;
            break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            gm->gameEvent = GAME_EVENT_INPUT;
            gm->mouseX = event.mouse.x;
            gm->mouseY = event.mouse.y;

            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            switch (event.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            }
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        if (redraw && al_is_event_queue_empty(queue))
        {

            DrawGame(gm);
            // al_draw_filled_triangle(35, 350, 85, 375, 35, 400, al_map_rgb_f(0, 1, 0));
            // al_draw_filled_rectangle(240, 260, 340, 340, al_map_rgba_f(0, 0, 0.5, 0.5));
            // al_draw_circle(450, 370, 30, al_map_rgb_f(1, 0, 1), 2);
            // al_draw_line(440, 110, 460, 210, al_map_rgb_f(1, 0, 0), 1);
            // al_draw_line(500, 220, 570, 200, al_map_rgb_f(1, 1, 0), 1);

            al_flip_display();

            redraw = false;
        }
    }

    //al_destroy_font(font); FIXME:
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}

int main(int argc, char *argv[])
{
    printf("Initializing!\n");
    // Audio
    initAudio();
    // Create a new game object
    GameManager *gm = InitGameManager();

    test_function_alegro(gm);
    return 0;
}
