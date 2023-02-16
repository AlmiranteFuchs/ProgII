
#include "gameAudio.h"
#include <stdio.h>
// Array of audio files

void initAudio(GameManager *gm)
{
    gm->audioSamples[0] = al_load_sample(audioPaths[AUDIO_MUSIC]);
    gm->audioSamples[1] = al_load_sample(audioPaths[AUDIO_MATCH]);
    gm->audioSamples[2] = al_load_sample(audioPaths[AUDIO_BOSS]);
    gm->audioSamples[3] = al_load_sample(audioPaths[AUDIO_MUSIC_MINIGAME]);
}

void playAudio(GameManager *gm, AudioPlayer *audio)
{
    al_play_sample(gm->audioSamples[audio->audio_num], audio->volume, 0.0, audio->speed, audio->loop == 1 ? ALLEGRO_PLAYMODE_LOOP : ALLEGRO_PLAYMODE_ONCE, NULL);
}

void stopAudio()
{
    al_stop_samples();
}

void destroyAudio(GameManager *gm)
{
    for (int i = 0; i < AUDIO_COUNT; i++)
    {
        al_destroy_sample(gm->audioSamples[i]);
    }
}

