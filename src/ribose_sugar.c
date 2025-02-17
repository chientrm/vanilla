#include <GL/glut.h>

float angleX = 0.0f, angleY = 0.0f;
float zoom = 1.0f;
int lastMouseX, lastMouseY;
int isDragging = 0; // Use int instead of bool

// Function to initialize GLUT
void initGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background to white
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

// Function to render a single atom
void renderAtom(float x, float y, float z, float radius, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(r, g, b);
    glutSolidSphere(radius, 20, 20);
    glPopMatrix();
}

// Function to render the ribose sugar molecule
void renderRiboseSugar() {
    // Carbon atoms (black)
    renderAtom(0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f); // C1
    renderAtom(0.2f, 0.2f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f); // C2
    renderAtom(0.4f, 0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f); // C3
    renderAtom(0.6f, 0.2f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f); // C4
    renderAtom(0.8f, 0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f); // C5

    // Oxygen atoms (blue)
    renderAtom(0.1f, 0.1f, 0.0f, 0.1f, 0.0f, 0.0f, 1.0f); // O1
    renderAtom(0.3f, 0.1f, 0.0f, 0.1f, 0.0f, 0.0f, 1.0f); // O2
    renderAtom(0.5f, 0.1f, 0.0f, 0.1f, 0.0f, 0.0f, 1.0f); // O3
    renderAtom(0.7f, 0.1f, 0.0f, 0.1f, 0.0f, 0.0f, 1.0f); // O4

    // Hydrogen atoms (red)
    renderAtom(-0.1f, 0.0f, 0.0f, 0.05f, 1.0f, 0.0f, 0.0f); // H1
    renderAtom(0.1f, -0.1f, 0.0f, 0.05f, 1.0f, 0.0f, 0.0f); // H2
    renderAtom(0.3f, -0.1f, 0.0f, 0.05f, 1.0f, 0.0f, 0.0f); // H3
    renderAtom(0.5f, -0.1f, 0.0f, 0.05f, 1.0f, 0.0f, 0.0f); // H4
    renderAtom(0.7f, -0.1f, 0.0f, 0.05f, 1.0f, 0.0f, 0.0f); // H5
}

// Mouse callback function
void mouse(int button, int state, int x, int y) {
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
            isDragging = 2;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            isDragging = 0;
        }
    }
}

// Motion callback function
void motion(int x, int y) {
    if (isDragging == 1) {
        angleX += (y - lastMouseY);
        angleY += (x - lastMouseX);
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    } else if (isDragging == 2) {
        zoom += (y - lastMouseY) * 0.01f;
        if (zoom < 0.1f) zoom = 0.1f; // Prevent zooming too close
        lastMouseY = y;
        glutPostRedisplay();
    }
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 2.0f / zoom, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f); // Adjust camera position
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    renderRiboseSugar();
    glutSwapBuffers();
}

// Reshape callback function
void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1; // Prevent division by zero
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ribose Sugar Molecule");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape); // Register reshape callback
    glutMouseFunc(mouse); // Register mouse callback
    glutMotionFunc(motion); // Register motion callback
    initGL();
    glutMainLoop();
    return 0;
}
