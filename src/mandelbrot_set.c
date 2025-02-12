#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 800
#define HEIGHT 800

void display();
void saveImage(const char *filename);

// Define the function to be called from main.c
void draw_mandelbrot_set() {
    int argc = 1;
    char *argv[1] = {(char *)"Something"};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Mandelbrot Set");
    glutDisplayFunc(display);
    glutMainLoop();
}

int main(int argc, char **argv) {
    draw_mandelbrot_set();
    return 0;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    double zoom = 1.0;
    double moveX = -0.5, moveY = 0.0;
    int maxIterations = 300;

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            double zx, zy, cX, cY;
            zx = zy = 0.0;
            cX = 1.5 * (x - WIDTH / 2) / (0.5 * zoom * WIDTH) + moveX;
            cY = (y - HEIGHT / 2) / (0.5 * zoom * HEIGHT) + moveY;
            int i;
            for (i = 0; i < maxIterations; i++) {
                double tmp = zx * zx - zy * zy + cX;
                zy = 2.0 * zx * zy + cY;
                zx = tmp;
                if (zx * zx + zy * zy > 4.0) break;
            }
            glColor3f((float)i / maxIterations, (float)i / maxIterations, (float)i / maxIterations);
            glVertex2f((x / (float)WIDTH) * 2 - 1, (y / (float)HEIGHT) * 2 - 1);
        }
    }

    glEnd();
    glFlush();
    saveImage("images/mandelbrot_set.png");
}

void saveImage(const char *filename) {
    // Create the images directory if it doesn't exist
    struct stat st = {0};
    if (stat("images", &st) == -1) {
        mkdir("images", 0700);
    }

    unsigned char *pixels = (unsigned char *)malloc(3 * WIDTH * HEIGHT);
    if (!pixels) {
        perror("Failed to allocate memory for image");
        return;
    }

    glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // Flip the image vertically
    for (int y = 0; y < HEIGHT / 2; y++) {
        for (int x = 0; x < WIDTH * 3; x++) {
            unsigned char temp = pixels[y * WIDTH * 3 + x];
            pixels[y * WIDTH * 3 + x] = pixels[(HEIGHT - 1 - y) * WIDTH * 3 + x];
            pixels[(HEIGHT - 1 - y) * WIDTH * 3 + x] = temp;
        }
    }

    if (!stbi_write_png(filename, WIDTH, HEIGHT, 3, pixels, WIDTH * 3)) {
        perror("Failed to write PNG file");
    }

    free(pixels);
}
