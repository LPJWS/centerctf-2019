CC=gcc
CFLAGS=-no-pie -m64 -fno-toplevel-reorder -Wl,-z,relro,-z,now 
outname=./on_the_edge
in_name=./main.c

all: release

release: CFLAGS += -s
release: executable

# make debug sanitize=1
debug: CFLAGS += -g -D DEBUG=1
	ifeq ($(sanitize), 1)
		CFLAGS += -fsanitize=undefined,address
	endif
debug: executable

executable:
	$(CC) $(CFLAGS) -o $(outname) $(in_name)