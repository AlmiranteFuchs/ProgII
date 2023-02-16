// Autor: B. Fuchs
// Info: Header file for game objects of the jewels game

#ifndef GAMECOMPONENTS_H
#define GAMECOMPONENTS_H

#include <stdlib.h>

// Structs for game objects
// The idea is to have a component based game object system

// Object transform
typedef struct
{
    float x;
    float y;
    float dx;
    float dy;
    float velocity;
    float acceleration;
    int moving_t;
    float angle;
    float angleSpeed;
    int visible;
    int moving;
} Transform;

// Object sprite
typedef struct
{
    int width;
    int height;
    int sprite_num;
} Sprite;

// Object audio player
typedef struct
{
    float volume;
    float speed;
    int playing;
    int loop;
    int audio_num;
    // AudioPlayer *audioPlayer;
} AudioPlayer;

// Object collider
// typedef struct {
//     int width;
//     int height;
//     int x;
//     int y;
// } Collider;

#endif // GAMECOMPONENTS_H