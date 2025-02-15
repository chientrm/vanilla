#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 800
#define HEIGHT 800
#define MAX_ITER 4

typedef struct {
    double x, y;
} Point;

void draw_square(unsigned char *image, Point p1, Point p2, unsigned char color[3]) {
    for (int y = (int)(p1.y * HEIGHT); y < (int)(p2.y * HEIGHT); y++) {
        for (int x = (int)(p1.x * WIDTH); x < (int)(p2.x * WIDTH); x++) {
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                int index = (y * WIDTH + x) * 3;
                image[index] = color[0];
                image[index + 1] = color[1];
                image[index + 2] = color[2];
            }
        }
    }
}

void menger_sponge(unsigned char *image, Point p1, Point p2, int iter, unsigned char color[3]) {
    if (iter == 0) {
        draw_square(image, p1, p2, color);
        return;
    }

    double dx = (p2.x - p1.x) / 3;
    double dy = (p2.y - p1.y) / 3;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == 1 && j == 1) continue;
            Point new_p1 = { p1.x + i * dx, p1.y + j * dy };
            Point new_p2 = { p1.x + (i + 1) * dx, p1.y + (j + 1) * dy };
            menger_sponge(image, new_p1, new_p2, iter - 1, color);
        }
    }
}

int main() {
    unsigned char *image = (unsigned char *)calloc(WIDTH * HEIGHT * 3, sizeof(unsigned char));
    if (!image) {
        fprintf(stderr, "Failed to allocate memory for image\n");
        return 1;
    }

    unsigned char color[3] = { 255, 255, 255 }; // White color

    Point p1 = { 0.1, 0.1 };
    Point p2 = { 0.9, 0.9 };

    menger_sponge(image, p1, p2, MAX_ITER, color);

    if (!stbi_write_png("images/menger_sponge.png", WIDTH, HEIGHT, 3, image, WIDTH * 3)) {
        fprintf(stderr, "Failed to write image\n");
        free(image);
        return 1;
    }

    free(image);
    return 0;
}
