// Autor: B. Fuchs
// Game objects header file

#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include "gameComponents.h"
#include "gameAudio.h"

// Game objects
// Jewels tile

// Represents a single tile, a jewel
#define TILE_SIZE 40
typedef struct
{
    Transform transform;
    Sprite sprite;
    int value;
    int falling;
    int selected;
    int real_posX;
    int real_posY;

    AudioPlayer audioPlayer;
} Tile;

// Moving foreground
typedef struct{
    Transform Transform;
    Sprite sprite;
} SimpleObject;

#endif // GAMEOBJECTS_H