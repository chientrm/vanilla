#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 800
#define HEIGHT 800
#define MAX_ITER 5

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

void peano_curve(unsigned char *image, Point p1, Point p2, int iter, unsigned char color[3]) {
    if (iter == 0) {
        draw_line(image, p1, p2, color);
        return;
    }

    double dx = (p2.x - p1.x) / 3;
    double dy = (p2.y - p1.y) / 3;

    Point p3 = { p1.x + dx, p1.y };
    Point p4 = { p1.x + 2 * dx, p1.y };
    Point p5 = { p1.x + 2 * dx, p1.y + dy };
    Point p6 = { p1.x + dx, p1.y + dy };
    Point p7 = { p1.x, p1.y + dy };
    Point p8 = { p1.x, p1.y + 2 * dy };
    Point p9 = { p1.x + dx, p1.y + 2 * dy };
    Point p10 = { p1.x + 2 * dx, p1.y + 2 * dy };
    Point p11 = { p1.x + 2 * dx, p1.y + 3 * dy };
    Point p12 = { p1.x + dx, p1.y + 3 * dy };
    Point p13 = { p1.x, p1.y + 3 * dy };

    peano_curve(image, p1, p3, iter - 1, color);
    peano_curve(image, p3, p4, iter - 1, color);
    peano_curve(image, p4, p5, iter - 1, color);
    peano_curve(image, p5, p6, iter - 1, color);
    peano_curve(image, p6, p7, iter - 1, color);
    peano_curve(image, p7, p8, iter - 1, color);
    peano_curve(image, p8, p9, iter - 1, color);
    peano_curve(image, p9, p10, iter - 1, color);
    peano_curve(image, p10, p11, iter - 1, color);
    peano_curve(image, p11, p12, iter - 1, color);
    peano_curve(image, p12, p13, iter - 1, color);
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

    peano_curve(image, p1, p2, MAX_ITER, color);

    if (!stbi_write_png("images/peano_curve.png", WIDTH, HEIGHT, 3, image, WIDTH * 3)) {
        fprintf(stderr, "Failed to write image\n");
        free(image);
        return 1;
    }

    free(image);
    return 0;
}
