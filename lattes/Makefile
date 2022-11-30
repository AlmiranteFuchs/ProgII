CFLAGS = -Wall -g 
LDFLAGS = -lm

debug: CFLAGS += -DDEBUG -g
debug: all

CC = gcc

# arquivos-objeto
	objects = index.o libs/string_lib.o libs/linked_lib.o src/entities.o utils/dir_lib.o
     
all: $(objects)
	$(CC) -o index $(objects) $(LDFLAGS)

string_lib.o: libs/string_lib.c
	$(CC) -c $(CFLAGS) libs/string_lib.c

entities.o: src/entities.c
	$(CC) -c $(CFLAGS) src/entities.c

linked_lib.o: libs/linked_lib.c
	$(CC) -c $(CFLAGS) libs/linked_lib.c

index.o: index.c
	$(CC) -c $(CFLAGS) index.c

dir_lib.o: utils/dir_lib.c
	$(CC) -c $(CFLAGS) utils/dir_lib.c

clean:
	rm -f $(objects) index
