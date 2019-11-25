CC=gcc
CFLAGS=-Wl,-z,relro,-z,now -fPIC -pie -m64 #-mpreferred-stack-boundary=4
outname=./master_of_cakes
in_name=./main.c

all:
	$(CC) $(CFLAGS) -o $(outname) $(in_name)

debug:
	$(CC) $(CFLAGS) -g -D DEBUG=1 -o $(outname) $(in_name)