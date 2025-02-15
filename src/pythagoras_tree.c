#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 800
#define HEIGHT 800
#define ORDER 10

void draw_line(unsigned char *image, int x1, int y1, int x2, int y2, unsigned char color[3]) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT) {
            int index = (y1 * WIDTH + x1) * 3;
            image[index] = color[0];
            image[index + 1] = color[1];
            image[index + 2] = color[2];
        }
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

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

void pythagoras_tree(unsigned char *image, int x, int y, int size, int angle, int depth, unsigned char color[3]) {
    if (depth == 0) return;

    int x2 = x + size * cos(angle * M_PI / 180.0);
    int y2 = y - size * sin(angle * M_PI / 180.0);
    draw_square(image, x, y - size, size, color);
    draw_line(image, x, y, x2, y2, color);

    int newSize = size * sqrt(2) / 2;
    pythagoras_tree(image, x2, y2, newSize, angle - 45, depth - 1, color);
    pythagoras_tree(image, x2, y2, newSize, angle + 45, depth - 1, color);
}

int main() {
    unsigned char *image = (unsigned char *)calloc(WIDTH * HEIGHT * 3, sizeof(unsigned char));
    if (!image) {
        fprintf(stderr, "Failed to allocate memory for image\n");
        return 1;
    }

    unsigned char color[3] = { 0, 255, 255 }; // Cyan color

    pythagoras_tree(image, WIDTH / 2, HEIGHT, WIDTH / 10, 90, ORDER, color);

    if (!stbi_write_png("images/pythagoras_tree.png", WIDTH, HEIGHT, 3, image, WIDTH * 3)) {
        fprintf(stderr, "Failed to write image\n");
        free(image);
        return 1;
    }

    free(image);
    return 0;
}
