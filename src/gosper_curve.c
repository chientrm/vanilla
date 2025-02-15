#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 800
#define HEIGHT 800
#define ORDER 4

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

void gosper_curve(unsigned char *image, int x1, int y1, int length, int depth, double angle, unsigned char color[3]) {
    if (depth == 0) {
        int x2 = x1 + length * cos(angle);
        int y2 = y1 + length * sin(angle);
        draw_line(image, x1, y1, x2, y2, color);
    } else {
        length /= sqrt(7);
        gosper_curve(image, x1, y1, length, depth - 1, angle, color);
        x1 += length * cos(angle);
        y1 += length * sin(angle);
        angle -= M_PI / 3;
        gosper_curve(image, x1, y1, length, depth - 1, angle, color);
        x1 += length * cos(angle);
        y1 += length * sin(angle);
        angle += 2 * M_PI / 3;
        gosper_curve(image, x1, y1, length, depth - 1, angle, color);
        x1 += length * cos(angle);
        y1 += length * sin(angle);
        angle -= M_PI / 3;
        gosper_curve(image, x1, y1, length, depth - 1, angle, color);
        x1 += length * cos(angle);
        y1 += length * sin(angle);
        angle -= M_PI / 3;
        gosper_curve(image, x1, y1, length, depth - 1, angle, color);
        x1 += length * cos(angle);
        y1 += length * sin(angle);
        angle += 2 * M_PI / 3;
        gosper_curve(image, x1, y1, length, depth - 1, angle, color);
        x1 += length * cos(angle);
        y1 += length * sin(angle);
        angle -= M_PI / 3;
        gosper_curve(image, x1, y1, length, depth - 1, angle, color);
    }
}

int main() {
    unsigned char *image = (unsigned char *)calloc(WIDTH * HEIGHT * 3, sizeof(unsigned char));
    if (!image) {
        fprintf(stderr, "Failed to allocate memory for image\n");
        return 1;
    }

    unsigned char color[3] = { 0, 255, 0 }; // Green color

    gosper_curve(image, WIDTH / 2, HEIGHT / 2, WIDTH / 2, ORDER, 0, color);

    if (!stbi_write_png("images/gosper_curve.png", WIDTH, HEIGHT, 3, image, WIDTH * 3)) {
        fprintf(stderr, "Failed to write image\n");
        free(image);
        return 1;
    }

    free(image);
    return 0;
}
