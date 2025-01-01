CC = gcc
CFLAGS = -Wall
SRC = src
OUT = build
TARGET = $(OUT)/program

SRCS = $(shell find $(SRC) -type f -name "*.c")

$(TARGET): $(SRCS)
	mkdir -p $(OUT)
	$(CC) $(CFLAGS) $^ -o $(TARGET)

clean:
	rm -rf $(OUT)
