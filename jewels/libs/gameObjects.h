// Autor: B. Fuchs
// Game objects header file

#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include "gameComponents.h"

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

// Enum for cenary
typedef enum
{
    CENARY_CLOUDS,
    CENARY_FOREGROUND,
    CENARY_OBJECTS,
} Cenary;

// Moving foreground
typedef struct
{
    Transform Transform;
    Sprite sprite;
    int direction; // 0 = left, 1 = right
    int active;
    Cenary cenary_type;
} SimpleObject;

// Boss
typedef struct
{
    Transform Transform;
    Sprite sprite;
    float internal_count;
    AudioPlayer audio;
} Boss;

#endif // GAMEOBJECTS_H