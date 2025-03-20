#include <GL/glut.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 800
#define HEIGHT 800
#define MAX_ITER 1000

void display();
void saveImage(const char *filename);

void display()
{
    static double time = 0.0; // Add a time variable for animation
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            double zx = 1.5 * (x - WIDTH / 2) / (0.5 * WIDTH);
            double zy = (y - HEIGHT / 2) / (0.5 * HEIGHT);
            complex double z = zx + zy * I;
            complex double c = -0.7 + 0.27015 * I + 0.1 * cos(time) + 0.1 * sin(time) * I; // Animate c
            int iter;
            for (iter = 0; iter < MAX_ITER; iter++)
            {
                if (cabs(z) > 2.0)
                    break;
                z = z * z + c;
            }
            double t = (double)iter / MAX_ITER;
            glColor3f(9 * (1 - t) * t * t * t, 15 * (1 - t) * (1 - t) * t * t, 8.5 * (1 - t) * (1 - t) * (1 - t) * t);
            glVertex2f((x - WIDTH / 2) / (WIDTH / 2.0), (y - HEIGHT / 2) / (HEIGHT / 2.0));
        }
    }

    glEnd();
    glFlush();
    time += 0.01; // Increment time for animation
    glutPostRedisplay();
}

void saveImage(const char *filename)
{
    // Create the images directory if it doesn't exist
    struct stat st = {0};
    if (stat("images", &st) == -1)
    {
        mkdir("images", 0700);
    }

    unsigned char *pixels = (unsigned char *)malloc(3 * WIDTH * HEIGHT);
    if (!pixels)
    {
        perror("Failed to allocate memory for image");
        return;
    }

    glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // Flip the image vertically
    for (int y = 0; y < HEIGHT / 2; y++)
    {
        for (int x = 0; x < WIDTH * 3; x++)
        {
            unsigned char temp = pixels[y * WIDTH * 3 + x];
            pixels[y * WIDTH * 3 + x] = pixels[(HEIGHT - 1 - y) * WIDTH * 3 + x];
            pixels[(HEIGHT - 1 - y) * WIDTH * 3 + x] = temp;
        }
    }

    if (!stbi_write_png(filename, WIDTH, HEIGHT, 3, pixels, WIDTH * 3))
    {
        perror("Failed to write PNG file");
    }

    free(pixels);
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Colored Julia Set Loop");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display); // Set display function as idle function
    glutMainLoop();
    return 0;
}
