CFLAGS = -Wall -g -std=c99 -D_BSD_SOURCE
LDFLAGS = -lm

debug: CFLAGS += -DDEBUG -g
debug: all

CC = gcc

# arquivos-objeto
	objects = lattes.o libs/string_lib.o libs/linked_lib.o src/entities.o utils/dir_lib.o
     
all: $(objects)
	$(CC) -o lattes $(objects) $(LDFLAGS)


string_lib.o: libs/string_lib.c
	$(CC) -c $(CFLAGS) libs/string_lib.c

entities.o: src/entities.c
	$(CC) -c $(CFLAGS) src/enlattestities.c

linked_lib.o: libs/linked_lib.c
	$(CC) -c $(CFLAGS) libs/linked_lib.c

lattes.o: lattes.c
	$(CC) -c $(CFLAGS) lattes.c

dir_lib.o: utils/dir_lib.c
	$(CC) -c $(CFLAGS) utils/dir_lib.c

clean:
	rm -f $(objects) index
