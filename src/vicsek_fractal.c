#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 800
#define HEIGHT 800

void drawVicsekFractal(unsigned char *image, int x, int y, int size, int order, unsigned char color[3]) {
    if (order == 0) {
        for (int i = x; i < x + size; i++) {
            for (int j = y; j < y + size; j++) {
                int index = (j * WIDTH + i) * 3;
                image[index] = color[0];
                image[index + 1] = color[1];
                image[index + 2] = color[2];
            }
        }
    } else {
        int newSize = size / 3;
        drawVicsekFractal(image, x, y, newSize, order - 1, color);
        drawVicsekFractal(image, x, y + 2 * newSize, newSize, order - 1, color);
        drawVicsekFractal(image, x + newSize, y + newSize, newSize, order - 1, color);
        drawVicsekFractal(image, x + 2 * newSize, y, newSize, order - 1, color);
        drawVicsekFractal(image, x + 2 * newSize, y + 2 * newSize, newSize, order - 1, color);
    }
}

int main() {
    unsigned char *image = (unsigned char *)calloc(WIDTH * HEIGHT * 3, sizeof(unsigned char));
    if (!image) {
        fprintf(stderr, "Failed to allocate memory for image\n");
        return 1;
    }

    unsigned char color[3] = { 255, 255, 255 }; // White color
    int order = 4; // Change the order to generate different levels of the fractal

    drawVicsekFractal(image, 0, 0, WIDTH, order, color);

    if (!stbi_write_png("images/vicsek_fractal.png", WIDTH, HEIGHT, 3, image, WIDTH * 3)) {
        fprintf(stderr, "Failed to write image\n");
        free(image);
        return 1;
    }

    free(image);
    return 0;
}
