#include "gameAudio.h"

// Array of audio files
static ALLEGRO_SAMPLE *audioFiles[MAX_AUDIO];

void _loadAudioFiles()
{
    audioFiles[0] = al_load_sample("assets/audio/1.wav");
    audioFiles[1] = al_load_sample("assets/audio/2.wav");
    audioFiles[2] = al_load_sample("assets/audio/3.wav");
    audioFiles[3] = al_load_sample("assets/audio/4.wav");
    audioFiles[4] = al_load_sample("assets/audio/5.wav");
}

void initAudio(Audio *audioPlayer)
{
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(16);

    audioPlayer->volume = 1.0;
    audioPlayer->playing = 0;
    audioPlayer->paused = 0;
    audioPlayer->stopped = 0;
    audioPlayer->path = NULL;
    audioPlayer->audioPlayer = NULL;
}
