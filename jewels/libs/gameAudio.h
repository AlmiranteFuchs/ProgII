
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "gameComponents.h"

#define MAX_AUDIO 10

typedef struct AudioPlayer {
    ALLEGRO_SAMPLE *audio[MAX_AUDIO];
    ALLEGRO_SAMPLE_INSTANCE *audioInstance[MAX_AUDIO];
    int audioCount;
} AudioPlayer;


// Initializes the audio system with allegro
void initAudio(AudioPlayer *audioPlayer);
void playAudio(AudioPlayer *audioPlayer, int audioIndex);
void stopAudio(AudioPlayer *audioPlayer, int audioIndex);
void destroyAudio(AudioPlayer *audioPlayer);