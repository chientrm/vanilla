#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 800
#define HEIGHT 800
#define MAX_ITER 1000

int main() {
    unsigned char img[WIDTH * HEIGHT * 3];
    double complex c, z;
    int x, y, n;
    double zx, zy, zx2, zy2;

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            c = (x - WIDTH / 2.0) * 4.0 / WIDTH + (y - HEIGHT / 2.0) * 4.0 / HEIGHT * I;
            z = 0;
            for (n = 0; n < MAX_ITER; n++) {
                zx = creal(z);
                zy = cimag(z);
                zx2 = zx * zx;
                zy2 = zy * zy;
                if (zx2 + zy2 > 4.0) break;
                z = zx2 - zy2 + c + 2.0 * fabs(zx * zy) * I;
            }
            int idx = (y * WIDTH + x) * 3;
            if (n == MAX_ITER) {
                img[idx] = img[idx + 1] = img[idx + 2] = 0;
            } else {
                img[idx] = (n % 256);
                img[idx + 1] = (n % 256) * 2;
                img[idx + 2] = (n % 256) * 4;
            }
        }
    }

    stbi_write_png("images/colored_burning_ship.png", WIDTH, HEIGHT, 3, img, WIDTH * 3);
    return 0;
}
