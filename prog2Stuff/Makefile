CFLAGS = -Wall -g
LDFLAGS = -lm			# Para all 

CC = gcc

objects = index.o src/test_write.o

all: $(objects)
	$(CC) -o index $(objects) $(LDFLAGS)

index.o: index.c
	$(CC) -c $(CFLAGS) index.c

test_write.o: src/test_write.c
	$(CC) -c $(CFLAGS) src/test_write.c

clean:
	rm -f $(objects) index