#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 800
#define HEIGHT 800
#define ITERATIONS 1000000

typedef struct {
    double x, y;
} Point;

void draw_point(unsigned char *image, Point p, unsigned char color[3]) {
    int x = (int)((p.x + 2.5) / 5.0 * WIDTH);
    int y = (int)((10 - p.y) / 10.0 * HEIGHT);

    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        int index = (y * WIDTH + x) * 3;
        image[index] = color[0];
        image[index + 1] = color[1];
        image[index + 2] = color[2];
    }
}

void barnsley_fern(Point *p) {
    double r = (double)rand() / RAND_MAX;
    Point new_p;
    if (r < 0.01) {
        new_p.x = 0;
        new_p.y = 0.16 * p->y;
    } else if (r < 0.86) {
        new_p.x = 0.85 * p->x + 0.04 * p->y;
        new_p.y = -0.04 * p->x + 0.85 * p->y + 1.6;
    } else if (r < 0.93) {
        new_p.x = 0.2 * p->x - 0.26 * p->y;
        new_p.y = 0.23 * p->x + 0.22 * p->y + 1.6;
    } else {
        new_p.x = -0.15 * p->x + 0.28 * p->y;
        new_p.y = 0.26 * p->x + 0.24 * p->y + 0.44;
    }
    *p = new_p;
}

void vicsek_fractal(Point *p) {
    double r = (double)rand() / RAND_MAX;
    Point new_p;
    if (r < 0.2) {
        new_p.x = p->x / 3;
        new_p.y = p->y / 3;
    } else if (r < 0.4) {
        new_p.x = p->x / 3 + 2.0 / 3.0;
        new_p.y = p->y / 3;
    } else if (r < 0.6) {
        new_p.x = p->x / 3;
        new_p.y = p->y / 3 + 2.0 / 3.0;
    } else if (r < 0.8) {
        new_p.x = p->x / 3 + 2.0 / 3.0;
        new_p.y = p->y / 3 + 2.0 / 3.0;
    } else {
        new_p.x = p->x / 3 + 1.0 / 3.0;
        new_p.y = p->y / 3 + 1.0 / 3.0;
    }
    *p = new_p;
}

void barnsley_vicsek_fractal(Point *p) {
    if ((double)rand() / RAND_MAX < 0.5) {
        barnsley_fern(p);
    } else {
        vicsek_fractal(p);
    }
}

int main() {
    unsigned char *image = (unsigned char *)calloc(WIDTH * HEIGHT * 3, sizeof(unsigned char));
    if (!image) {
        fprintf(stderr, "Failed to allocate memory for image\n");
        return 1;
    }

    unsigned char color[3] = { 0, 255, 0 }; // Green color

    Point p = { 0, 0 };
    for (int i = 0; i < ITERATIONS; i++) {
        barnsley_vicsek_fractal(&p);
        draw_point(image, p, color);
    }

    if (!stbi_write_png("images/barnsley_vicsek_fractal.png", WIDTH, HEIGHT, 3, image, WIDTH * 3)) {
        fprintf(stderr, "Failed to write image\n");
        free(image);
        return 1;
    }

    free(image);
    return 0;
}
