CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O3
SRC_DIR = src/engine
OUTPUT = src/chess
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)
	rm -f $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(OUTPUT)
    python3 src/UI/ui.py

leak:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         ./chess Ba5

clean:
	rm -f $(OBJS) $(OUTPUT)

generate_magic:
	$(CC) $(CFLAGS) -o src/engine/a.exe src/engine/magicboards.c
	src/engine/a.exe
	rm src/engine/a.exe

.PHONY: all clean