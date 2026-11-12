CC = gcc
CFLAGS = -Wall -O2
BINS = main_file reverse_str length_str ascii_add_2_str sort_str capitalize_str

.PHONY: all clean

all: $(BINS)

main_file: main_file.c
	$(CC) $(CFLAGS) -o $@ $<

reverse_str: reverse_str.c
	$(CC) $(CFLAGS) -o $@ $<

length_str: length_str.c
	$(CC) $(CFLAGS) -o $@ $<

ascii_add_2_str: ascii_add_2_str.c
	$(CC) $(CFLAGS) -o $@ $<

sort_str: sort_str.c
	$(CC) $(CFLAGS) -o $@ $<

capitalize_str: capitalize_str.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BINS) input.txt output.txt
