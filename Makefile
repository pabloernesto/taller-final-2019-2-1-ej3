src = $(wildcard src/*.c)

obj = $(src:.c=.o)

CFLAGS = -std=c99 -Wall -g

# Use LDFLAGS for options that include libraries
# LDFLAGS = -lGL -lglut -lpng -lz -lm

.PHONY: all
all: solution nros2bytes.dat

.PHONY: run
run: all
	./solution

solution: $(obj)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

nros2bytes.dat: dataset.sh
	./dataset.sh

.PHONY: clean
clean:
	rm -f $(obj) solution nros2bytes.dat
