

#ifndef GAMEAUDIO_H
#define GAMEAUDIO_H

#include "gameLogic.h"

// Enum for audio for path
typedef enum
{
    AUDIO_MUSIC,
    AUDIO_MATCH,
    AUDIO_BOSS,
    AUDIO_MUSIC_MINIGAME,
    AUDIO_BOMB,
    AUDIO_SELECT,
    AUDIO_DESELECT,
    AUDIO_LEVEL_UP,
} AudioPath;

static char *const audioPaths[] =
    {
        [AUDIO_MUSIC] = "resources/audio/music/w_day_m.wav",
        [AUDIO_MATCH] = "resources/audio/SFX/match.wav",
        [AUDIO_BOSS] = "resources/audio/SFX/boss.wav",
        [AUDIO_MUSIC_MINIGAME] = "resources/audio/music/boss_m.wav",
        [AUDIO_BOMB] = "resources/audio/SFX/col_reset.wav",
        [AUDIO_SELECT] = "resources/audio/SFX/select.wav",
        [AUDIO_DESELECT] = "resources/audio/SFX/deselect.wav",
        [AUDIO_LEVEL_UP] = "resources/audio/SFX/lvl_up.wav",


};
//          [AUDIO_MATCH] = "assets/audio/match.wav",
//         [AUDIO_SWAP] = "assets/audio/swap.wav",
//         [AUDIO_SELECT] = "assets/audio/select.wav",
//         [AUDIO_SCORE] = "assets/audio/score.wav",

// // Initializes the audio system with allegro
void initAudio(GameManager *gm);
void destroyAudio(GameManager *gm);
void playAudio(GameManager *gm, AudioPlayer *audio);
void stopAudio();

#endif // GAMEAUDIO_H