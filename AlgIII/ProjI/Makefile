CFLAGS = -Wall -g
LDFLAGS = -lm

debug: CFLAGS += -DDEBUG -g
debug: all

CC = gcc

# arquivos-objeto
	objects = libs/avl_lib.o myavl.o
     
all: $(objects)
	$(CC) -o myavl $(objects) $(LDFLAGS)

myavl.o: myavl.c
	$(CC) -c $(CFLAGS) myavl.c

avl_lib.o: libs/avl_lib.c
	$(CC) -c $(CFLAGS) libs/avl_lib.c

clean:
	rm -f $(objects) myavl
