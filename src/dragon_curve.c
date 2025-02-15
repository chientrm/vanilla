#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 800
#define HEIGHT 800
#define MAX_ITER 15

typedef struct {
    double x, y;
} Point;

void draw_line(unsigned char *image, Point p1, Point p2, unsigned char color[3]) {
    int x1 = (int)(p1.x * WIDTH);
    int y1 = (int)(p1.y * HEIGHT);
    int x2 = (int)(p2.x * WIDTH);
    int y2 = (int)(p2.y * HEIGHT);

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

void dragon_curve(unsigned char *image, Point p1, Point p2, int iter, unsigned char color[3]) {
    if (iter == 0) {
        draw_line(image, p1, p2, color);
        return;
    }

    Point mid = { 
        (p1.x + p2.x) / 2 + (p2.y - p1.y) / 2, 
        (p1.y + p2.y) / 2 - (p2.x - p1.x) / 2 
    };

    dragon_curve(image, p1, mid, iter - 1, color);
    dragon_curve(image, mid, p2, iter - 1, color);
}

int main() {
    unsigned char *image = (unsigned char *)calloc(WIDTH * HEIGHT * 3, sizeof(unsigned char));
    if (!image) {
        fprintf(stderr, "Failed to allocate memory for image\n");
        return 1;
    }

    unsigned char color[3] = { 255, 255, 255 }; // White color

    Point p1 = { 0.25, 0.5 };
    Point p2 = { 0.75, 0.5 };

    dragon_curve(image, p1, p2, MAX_ITER, color);

    if (!stbi_write_png("images/dragon_curve.png", WIDTH, HEIGHT, 3, image, WIDTH * 3)) {
        fprintf(stderr, "Failed to write image\n");
        free(image);
        return 1;
    }

    free(image);
    return 0;
}
