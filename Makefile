CC = gcc
CFLAGS = -Wall -g -s
SRCS = a6.c avl.c collision.c
OBJS = $(SRCS:.c=.o)
TARGET = a6
	
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)