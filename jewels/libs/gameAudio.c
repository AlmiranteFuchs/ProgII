#include "gameAudio.h"
#include <stdio.h>

// Array of audio files

static ALLEGRO_SAMPLE *audioSamples[MAX_AUDIO];

void _loadAudioFiles()
{
    audioSamples[0] = al_load_sample(audioPaths[AUDIO_MUSIC]);
    audioSamples[1] = al_load_sample(audioPaths[AUDIO_MATCH]);
}

void initAudio()
{
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(16);
 

    _loadAudioFiles();
}

void playAudio(AudioPlayer *audio)
{
    al_play_sample(audioSamples[audio->audio_num], audio->volume, 0.0, audio->speed, audio->loop == 1 ? ALLEGRO_PLAYMODE_LOOP : ALLEGRO_PLAYMODE_ONCE, NULL);
}
