CC=gcc
CFLAGS=-m64 -O0
LIBS=-lpthread
outname=./fileless

# nasm -f elf64 fileless.asm && ld fileless.o -o fileless_out && objcopy -O binary --only-section=.text -I elf64-x86-64 ./fileless_out opcodes.bin && hexdump -v -e '"\\""x" 1/1 "%02x" ""' opcodes.bin

all: release

debug: CFLAGS += -g -D DEBUG
debug: clean
debug: executable

release: CFLAGS += -s
release: clean
release: executable

executable: main.o antidebug.o
	$(CC) -o $(outname) main.o antidebug.o $(LIBS)

main.o: main.c antidebug.h
	$(CC) $(CFLAGS) -c main.c -lpthread $(LIBS)

antidebug.o: antidebug.c antidebug.h
	$(CC) $(CFLAGS) -c antidebug.c -lpthread $(LIBS)

clean:
	rm -rf *.o