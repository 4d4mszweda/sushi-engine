CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O3
SRC_DIR = src/engine
OUTPUT = chess
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)
	rm -f $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

leak:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         ./chess Ba5

clean:
	rm -f $(OBJS) $(OUTPUT)

.PHONY: all clean