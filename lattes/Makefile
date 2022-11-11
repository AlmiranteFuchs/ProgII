CFLAGS = -Wall -g
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = index.o string_lib.o
     
all: index.o string_lib.o entities.o
	$(CC) -o index index.o string_lib.o entities.o $(LDFLAGS)

string_lib.o: libs/string_lib.c
	$(CC) -c $(CFLAGS) libs/string_lib.c

entities.o: src/entities.c
	$(CC) -c $(CFLAGS) src/entities.c

index.o: index.c
	$(CC) -c $(CFLAGS) index.c

clean:
	rm -f $(objects) index
