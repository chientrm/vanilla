#include <GL/glut.h>

float angleX = 0.0f;
float angleY = 0.0f;
float zoom = -5.0f;
int lastMouseX, lastMouseY;
int isDragging = 0;

void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightPos[] = { 1.0f, 1.0f, 1.0f, 0.0f }; // Directional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    // Draw the cube
    glColor3f(0.5f, 0.5f, 0.5f);
    glutSolidCube(1.0);

    glutSwapBuffers();
}

void reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void mouseMotion(int x, int y) {
    if (isDragging) {
        angleX += (y - lastMouseY);
        angleY += (x - lastMouseX);
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = 1;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            isDragging = 0;
        }
    } else if (button == GLUT_MIDDLE_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = 1;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            isDragging = 0;
        }
    } else if (button == 3) { // Scroll up
        zoom += 0.5f;
        glutPostRedisplay();
    } else if (button == 4) { // Scroll down
        zoom -= 0.5f;
        glutPostRedisplay();
    }
}

void keyboard(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            angleX -= 5.0f;
            break;
        case GLUT_KEY_DOWN:
            angleX += 5.0f;
            break;
        case GLUT_KEY_LEFT:
            angleY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            angleY += 5.0f;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Cube with Mouse Navigation");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseButton);
    glutSpecialFunc(keyboard);
    initGL();
    glutMainLoop();
    return 0;
}
