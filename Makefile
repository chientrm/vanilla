CC = gcc
CFLAGS = -Wall -O2 -Iinclude
LDFLAGS = -lm -lGLU -lglut -lGL
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
IMG_DIR = images

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
TARGETS = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%, $(SRC_FILES))

all: $(TARGETS)

$(BIN_DIR)/%: $(OBJ_DIR)/%.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGETS)
	rm -f $(IMG_DIR)/*.png

.PHONY: all clean