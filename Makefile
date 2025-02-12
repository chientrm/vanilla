CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
LDFLAGS = -lGL -lGLU -lglut -lm

# Define source files for each program
SRC_MAIN = src/main.c src/utils.c
SRC_JULIA = src/julia_set.c
SRC_MANDELBROT = src/mandelbrot_set.c
SRC_BURNING_SHIP = src/burning_ship_set.c
SRC_COLORED_JULIA = src/colored_julia_set.c

# Define object files for each program
OBJ_MAIN = $(SRC_MAIN:src/%.c=obj/%.o)
OBJ_JULIA = $(SRC_JULIA:src/%.c=obj/%.o)
OBJ_MANDELBROT = $(SRC_MANDELBROT:src/%.c=obj/%.o)
OBJ_BURNING_SHIP = $(SRC_BURNING_SHIP:src/%.c=obj/%.o)
OBJ_COLORED_JULIA = $(SRC_COLORED_JULIA:src/%.c=obj/%.o)

# Define targets for each program
TARGET_MAIN = bin/my-c-project
TARGET_JULIA = bin/julia-set
TARGET_MANDELBROT = bin/mandelbrot-set
TARGET_BURNING_SHIP = bin/burning-ship-set
TARGET_COLORED_JULIA = bin/colored-julia-set

all: $(TARGET_MAIN) $(TARGET_JULIA) $(TARGET_MANDELBROT) $(TARGET_BURNING_SHIP) $(TARGET_COLORED_JULIA)

$(TARGET_MAIN): $(OBJ_MAIN)
	@mkdir -p bin
	$(CC) -o $@ $^

$(TARGET_JULIA): $(OBJ_JULIA)
	@mkdir -p bin images
	$(CC) -o $@ $^ $(LDFLAGS)

$(TARGET_MANDELBROT): $(OBJ_MANDELBROT)
	@mkdir -p bin images
	$(CC) -o $@ $^ $(LDFLAGS)

$(TARGET_BURNING_SHIP): $(OBJ_BURNING_SHIP)
	@mkdir -p bin images
	$(CC) -o $@ $^ $(LDFLAGS)

$(TARGET_COLORED_JULIA): $(OBJ_COLORED_JULIA)
	@mkdir -p bin images
	$(CC) -o $@ $^ $(LDFLAGS)

colored_julia_set: src/colored_julia_set.c
	gcc -o colored_julia_set src/colored_julia_set.c -lGL -lGLU -lglut -lm

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f obj/*.o
	rm -f $(TARGET_MAIN) $(TARGET_JULIA) $(TARGET_MANDELBROT) $(TARGET_BURNING_SHIP) $(TARGET_COLORED_JULIA) $(TARGET_COLORED_JULIA)
	rm -rf bin obj images

.PHONY: all clean