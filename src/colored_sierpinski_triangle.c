#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdlib.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 800

void drawSierpinski(unsigned char *image, int x, int y, int size, int depth) {
    if (depth == 0) {
        // Draw a filled triangle
        int r = rand() % 256;
        int g = rand() % 256;
        int b = rand() % 256;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < i; ++j) {
                int px = x + j - i / 2;
                int py = y + i;
                if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
                    int index = (py * WIDTH + px) * 3;
                    image[index] = r;
                    image[index + 1] = g;
                    image[index + 2] = b;
                }
            }
        }
    } else {
        // Recursively draw smaller triangles
        drawSierpinski(image, x, y, size / 2, depth - 1);
        drawSierpinski(image, x - size / 4, y + size / 2, size / 2, depth - 1);
        drawSierpinski(image, x + size / 4, y + size / 2, size / 2, depth - 1);
    }
}

int main() {
    unsigned char *image = (unsigned char *)malloc(WIDTH * HEIGHT * 3);
    if (!image) {
        return 1;
    }

    srand(time(NULL));
    for (int i = 0; i < WIDTH * HEIGHT * 3; ++i) {
        image[i] = 0;
    }

    drawSierpinski(image, WIDTH / 2, 50, WIDTH / 2, 6);

    stbi_write_png("images/colored_sierpinski_triangle.png", WIDTH, HEIGHT, 3, image, WIDTH * 3);

    free(image);
    return 0;
}
