#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void drawSierpinski(unsigned char *image, int n, int size, int width, int height) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j <= i; j++) {
            if ((i & j) == j) {
                int x = (n - i - 1 + 2 * j) * size;
                int y = i * size;
                for (int dy = 0; dy < size; dy++) {
                    for (int dx = 0; dx < size; dx++) {
                        int index = 4 * ((y + dy) * width + (x + dx));
                        image[index] = 0;     // R
                        image[index + 1] = 0; // G
                        image[index + 2] = 0; // B
                        image[index + 3] = 255; // A
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_rows>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Number of rows must be a positive integer.\n");
        return 1;
    }

    const char *output_file = "images/sierpinski_triangle.png";
    int size = 10;
    int width = 2 * n * size;
    int height = n * size;

    unsigned char *image = (unsigned char *)malloc(4 * width * height);
    if (!image) {
        fprintf(stderr, "Failed to allocate memory for image.\n");
        return 1;
    }

    // Set background to white
    for (int i = 0; i < 4 * width * height; i += 4) {
        image[i] = 255;
        image[i + 1] = 255;
        image[i + 2] = 255;
        image[i + 3] = 255;
    }

    drawSierpinski(image, n, size, width, height);

    if (!stbi_write_png(output_file, width, height, 4, image, width * 4)) {
        fprintf(stderr, "Failed to write image to %s\n", output_file);
        free(image);
        return 1;
    }

    free(image);
    return 0;
}
