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

void rotateImage45Degrees(unsigned char *src, unsigned char *dst, int width, int height) {
    int newWidth = width * 1.414; // sqrt(2) * width
    int newHeight = height * 1.414; // sqrt(2) * height
    int cx = width / 2;
    int cy = height / 2;
    int ncx = newWidth / 2;
    int ncy = newHeight / 2;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int nx = (x - cx) * 0.707 - (y - cy) * 0.707 + ncx;
            int ny = (x - cx) * 0.707 + (y - cy) * 0.707 + ncy;
            if (nx >= 0 && nx < newWidth && ny >= 0 && ny < newHeight) {
                int srcIndex = (y * width + x) * 3;
                int dstIndex = (ny * newWidth + nx) * 3;
                dst[dstIndex] = src[srcIndex];
                dst[dstIndex + 1] = src[srcIndex + 1];
                dst[dstIndex + 2] = src[srcIndex + 2];
            }
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
    int order = 4; // Change the order to generate different levels of the fractal

    drawVicsekFractal(image, 0, 0, WIDTH, order, color);

    int newWidth = WIDTH * 1.414; // sqrt(2) * WIDTH
    int newHeight = HEIGHT * 1.414; // sqrt(2) * HEIGHT
    unsigned char *rotatedImage = (unsigned char *)calloc(newWidth * newHeight * 3, sizeof(unsigned char));
    if (!rotatedImage) {
        fprintf(stderr, "Failed to allocate memory for rotated image\n");
        free(image);
        return 1;
    }

    rotateImage45Degrees(image, rotatedImage, WIDTH, HEIGHT);

    if (!stbi_write_png("images/vicsek_fractal_rotated.png", newWidth, newHeight, 3, rotatedImage, newWidth * 3)) {
        fprintf(stderr, "Failed to write image\n");
        free(image);
        free(rotatedImage);
        return 1;
    }

    free(image);
    free(rotatedImage);
    return 0;
}
