CFLAGS = -Wall -g -std=c99 -I ./include/
LDFLAGS = `pkg-config allegro-5 allegro_font-5 --libs --cflags` -lm
#EOPTION = `pkg-config --cflags --libs allegro-5.0 allegro_primitives-5.0 allegro_font-5.0 allegro_audio-5.0 allegro_ttf-5.0 allegro_image-5.0 allegro_acodec-5.0` -lm

CC = gcc

objects = jewels.o

all: $(objects)
	$(CC) -o jewels $(objects) $(LDFLAGS) $(EOPTION)

jewels.o: jewels.c
	$(CC) -c $(CFLAGS) jewels.c 

#remove os arquivos temporários (mantém o executável)
clean:
	rm -f jewels $(objects)

#remove tudo, deixando somente os fontes
purge: clean
	rm -f *.o