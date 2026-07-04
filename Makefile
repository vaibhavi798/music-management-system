CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRCS = main.c song.c history.c setops.c player.c fileio.c input.c util.c
OBJS = $(SRCS:.c=.o)
TARGET = music_manager

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
