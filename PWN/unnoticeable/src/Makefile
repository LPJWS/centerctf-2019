CC=gcc
CFLAGS=-Wl,-z,relro,-z,now -no-pie -m64 #-mpreferred-stack-boundary=4 -fPIC -pie
outname=./unnoticeable
in_name=./main.c

all:
	$(CC) $(CFLAGS) -o $(outname) $(in_name)

debug:
	$(CC) $(CFLAGS) -g -o $(outname) $(in_name)