#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 800
#define HEIGHT 800
#define MAX_DEPTH 10

void draw_circle(unsigned char *image, int cx, int cy, int radius, unsigned char color[3]) {
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        for (int i = -x; i <= x; i++) {
            if (cx + i >= 0 && cx + i < WIDTH && cy + y >= 0 && cy + y < HEIGHT) {
                int index = ((cy + y) * WIDTH + (cx + i)) * 3;
                image[index] = color[0];
                image[index + 1] = color[1];
                image[index + 2] = color[2];
            }
            if (cx + i >= 0 && cx + i < WIDTH && cy - y >= 0 && cy - y < HEIGHT) {
                int index = ((cy - y) * WIDTH + (cx + i)) * 3;
                image[index] = color[0];
                image[index + 1] = color[1];
                image[index + 2] = color[2];
            }
        }
        for (int i = -y; i <= y; i++) {
            if (cx + i >= 0 && cx + i < WIDTH && cy + x >= 0 && cy + x < HEIGHT) {
                int index = ((cy + x) * WIDTH + (cx + i)) * 3;
                image[index] = color[0];
                image[index + 1] = color[1];
                image[index + 2] = color[2];
            }
            if (cx + i >= 0 && cx + i < WIDTH && cy - x >= 0 && cy - x < HEIGHT) {
                int index = ((cy - x) * WIDTH + (cx + i)) * 3;
                image[index] = color[0];
                image[index + 1] = color[1];
                image[index + 2] = color[2];
            }
        }
        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void apollonian_gasket(unsigned char *image, double x1, double y1, double r1, double x2, double y2, double r2, double x3, double y3, double r3, int depth, unsigned char color[3]) {
    if (depth == 0) return;

    draw_circle(image, (int)x1, (int)y1, (int)r1, color);
    draw_circle(image, (int)x2, (int)y2, (int)r2, color);
    draw_circle(image, (int)x3, (int)y3, (int)r3, color);

    double k1 = 1.0 / r1, k2 = 1.0 / r2, k3 = 1.0 / r3;
    double k4 = k1 + k2 + k3 + 2 * sqrt(k1 * k2 + k2 * k3 + k3 * k1);
    double r4 = 1.0 / k4;

    double a1 = x1 / r1, a2 = x2 / r2, a3 = x3 / r3;
    double b1 = y1 / r1, b2 = y2 / r2, b3 = y3 / r3;

    double a4 = (a1 + a2 + a3 + 2 * sqrt(a1 * a2 + a2 * a3 + a3 * a1)) / k4;
    double b4 = (b1 + b2 + b3 + 2 * sqrt(b1 * b2 + b2 * b3 + b3 * b1)) / k4;

    double x4 = a4 * r4, y4 = b4 * r4;

    apollonian_gasket(image, x1, y1, r1, x2, y2, r2, x4, y4, r4, depth - 1, color);
    apollonian_gasket(image, x2, y2, r2, x3, y3, r3, x4, y4, r4, depth - 1, color);
    apollonian_gasket(image, x3, y3, r3, x1, y1, r1, x4, y4, r4, depth - 1, color);
}

int main() {
    unsigned char *image = (unsigned char *)calloc(WIDTH * HEIGHT * 3, sizeof(unsigned char));
    if (!image) {
        fprintf(stderr, "Failed to allocate memory for image\n");
        return 1;
    }

    unsigned char color[3] = { 255, 255, 255 }; // White color

    double r1 = WIDTH / 4.0, r2 = WIDTH / 4.0, r3 = WIDTH / 4.0;
    double x1 = WIDTH / 2.0, y1 = HEIGHT / 2.0 - r1;
    double x2 = WIDTH / 2.0 - r2 * sqrt(3) / 2.0, y2 = HEIGHT / 2.0 + r2 / 2.0;
    double x3 = WIDTH / 2.0 + r3 * sqrt(3) / 2.0, y3 = HEIGHT / 2.0 + r3 / 2.0;

    apollonian_gasket(image, x1, y1, r1, x2, y2, r2, x3, y3, r3, MAX_DEPTH, color);

    if (!stbi_write_png("images/apollonian_gasket.png", WIDTH, HEIGHT, 3, image, WIDTH * 3)) {
        fprintf(stderr, "Failed to write image\n");
        free(image);
        return 1;
    }

    free(image);
    return 0;
}
