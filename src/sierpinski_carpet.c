#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 800
#define HEIGHT 800
#define ORDER 4

void draw_square(unsigned char *image, int x, int y, int size, unsigned char color[3]) {
    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            if (i >= 0 && i < WIDTH && j >= 0 && j < HEIGHT) {
                int index = (j * WIDTH + i) * 3;
                image[index] = color[0];
                image[index + 1] = color[1];
                image[index + 2] = color[2];
            }
        }
    }
}

void sierpinski_carpet(unsigned char *image, int x, int y, int size, int depth, unsigned char color[3]) {
    if (depth == 0) {
        draw_square(image, x, y, size, color);
    } else {
        int newSize = size / 3;
        sierpinski_carpet(image, x, y, newSize, depth - 1, color);
        sierpinski_carpet(image, x + newSize, y, newSize, depth - 1, color);
        sierpinski_carpet(image, x + 2 * newSize, y, newSize, depth - 1, color);
        sierpinski_carpet(image, x, y + newSize, newSize, depth - 1, color);
        sierpinski_carpet(image, x + 2 * newSize, y + newSize, newSize, depth - 1, color);
        sierpinski_carpet(image, x, y + 2 * newSize, newSize, depth - 1, color);
        sierpinski_carpet(image, x + newSize, y + 2 * newSize, newSize, depth - 1, color);
        sierpinski_carpet(image, x + 2 * newSize, y + 2 * newSize, newSize, depth - 1, color);
    }
}

int main() {
    unsigned char *image = (unsigned char *)calloc(WIDTH * HEIGHT * 3, sizeof(unsigned char));
    if (!image) {
        fprintf(stderr, "Failed to allocate memory for image\n");
        return 1;
    }

    unsigned char color[3] = { 255, 0, 255 }; // Magenta color

    sierpinski_carpet(image, WIDTH / 3, HEIGHT / 3, WIDTH / 3, ORDER, color);

    if (!stbi_write_png("images/sierpinski_carpet.png", WIDTH, HEIGHT, 3, image, WIDTH * 3)) {
        fprintf(stderr, "Failed to write image\n");
        free(image);
        return 1;
    }

    free(image);
    return 0;
}
