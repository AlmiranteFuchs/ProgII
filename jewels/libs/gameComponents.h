// Autor: B. Fuchs
// Info: Header file for game objects of the jewels game

#ifndef GAMECOMPONENTS_H
#define GAMECOMPONENTS_H

#include <stdlib.h>

// Structs for game objects
// The idea is to have a component based game object system

// Object transform
typedef struct {
    int x;
    int y;
    int dx;
    int dy;
    int velocity;
    int angle;
    int angleSpeed;
    int visible;
    int moving;
} Transform;

// Object sprite
typedef struct {
    int width;
    int height;
    int color;
    char* path;
} Sprite;

// Object collider
// typedef struct {
//     int width;
//     int height;
//     int x;
//     int y;
// } Collider;




#endif // GAMECOMPONENTS_H