CFLAGS = -Wall -g -std=c99 -I ./include/
LDFLAGS = `pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5 --libs --cflags` -lm
#EOPTION = `pkg-config --cflags --libs allegro-5.0 allegro_primitives-5.0 allegro_font-5.0 allegro_audio-5.0 allegro_ttf-5.0 allegro_image-5.0 allegro_acodec-5.0` -lm

CC = gcc

objects = jewels.o gameObjects.o gameGraphics.o gameLogic.o gameAudio.o

all: $(objects)
	$(CC) -o jewels $(objects) $(LDFLAGS) $(EOPTION)

jewels.o: clean jewels.c 
	$(CC) -c $(CFLAGS) jewels.c 

gameObjects.o: libs/gameObjects.c
	$(CC) -c $(CFLAGS) libs/gameObjects.c

gameGraphics.o: libs/gameGraphics.c
	$(CC) -c $(CFLAGS) libs/gameGraphics.c

gameLogic.o: libs/gameLogic.c
	$(CC) -c $(CFLAGS) libs/gameLogic.c

gameAudio.o: libs/gameAudio.c
	$(CC) -c $(CFLAGS) libs/gameAudio.c

#remove os arquivos temporários (mantém o executável)
clean:
	rm -f jewels $(objects)

#remove tudo, deixando somente os fontes
purge: clean
	rm -f *.o