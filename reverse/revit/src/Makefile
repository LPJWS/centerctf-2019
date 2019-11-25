CC=gcc
CFLAGS=-m64 -O0
outname=./revit

all: release

debug: CFLAGS += -g -D DEBUG
debug: clean
debug: executable

release: CFLAGS += -s
release: clean
release: executable

executable: main.o cryptor.o
	$(CC) -o $(outname) main.o cryptor.o

main.o: main.c cryptor.h
	$(CC) $(CFLAGS) -c main.c

cryptor.o: cryptor.c cryptor.h
	$(CC) $(CFLAGS) -c cryptor.c

clean:
	rm -rf *.o