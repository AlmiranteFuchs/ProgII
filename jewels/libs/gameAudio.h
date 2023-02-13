
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "gameComponents.h"

#ifndef GAMEAUDIO_H
#define GAMEAUDIO_H

#define MAX_AUDIO 5

// Enum for audio for path
typedef enum
{
    AUDIO_MUSIC,
    AUDIO_MATCH,
    AUDIO_SWAP,
    AUDIO_SELECT,
    AUDIO_SCORE
} AudioPath;

static char *const audioPaths[] =
    {
        [AUDIO_MUSIC] = "resources/audio/music/w_day_m.wav",
        [AUDIO_MATCH] = "resources/audio/SFX/match.wav",

};
//          [AUDIO_MATCH] = "assets/audio/match.wav",
//         [AUDIO_SWAP] = "assets/audio/swap.wav",
//         [AUDIO_SELECT] = "assets/audio/select.wav",
//         [AUDIO_SCORE] = "assets/audio/score.wav",

// // Initializes the audio system with allegro
void initAudio();
void playAudio(AudioPlayer *audio);

#endif // GAMEAUDIO_H