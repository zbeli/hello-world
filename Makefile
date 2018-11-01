CC = gcc
CFLAGS = -I.

main: main.o str.o
	$(CC) -o main main.o str.o

clean :
	rm -f *.o main
