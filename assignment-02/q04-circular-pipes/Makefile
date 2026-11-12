CC = gcc
CFLAGS = -Wall -O2

.PHONY: all clean

all: circular-pipes

circular-pipes: circular-pipes.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f circular-pipes log_A.txt log_B.txt log_C.txt log_D.txt
