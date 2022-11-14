CC      = cc
CFLAGS  = -g -Wall
INCLUDE = -I/usr/include/PCSC

all: demo

clean:
	rm -f demo *.o

demo : demo.c
	$(CC) $(CFLAGS) -o $@ $^ $(INCLUDE) -lpcsclite

%.o : %.c
	$(CC) $(CFLAGS) -c $< $(INCLUDE)
