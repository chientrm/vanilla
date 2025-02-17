#include <GL/glut.h>
#include <stdlib.h>

typedef struct {
    float x, y, z;
    float radius;
    float r, g, b;
} Atom;

Atom* atoms = NULL;
size_t atomCount = 0;
size_t atomCapacity = 0;

float angleX = 0.0f, angleY = 0.0f;
float zoom = 1.0f;
int lastMouseX, lastMouseY;
int isDragging = 0; // Use int instead of bool
int selectedAtomIndex = -1; // Index of the selected atom

// Function to initialize GLUT
void initGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background to white
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lightPos[] = {0.0f, 0.0f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    GLfloat lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
}

// Function to render a single atom
void renderAtom(const Atom* atom, int isSelected) {
    glPushMatrix();
    glTranslatef(atom->x, atom->y, atom->z);
    if (isSelected) {
        glColor3f(1.0f, 1.0f, 0.0f); // Highlight selected atom with yellow color
        glutWireSphere(atom->radius * 1.2, 20, 20); // Draw a wireframe sphere around the selected atom
    }
    glColor3f(atom->r, atom->g, atom->b);
    glutSolidSphere(atom->radius, 20, 20);
    glPopMatrix();
}

// Function to render all atoms
void renderAtoms() {
    for (size_t i = 0; i < atomCount; ++i) {
        renderAtom(&atoms[i], i == selectedAtomIndex);
    }
}

// Function to add a new atom
void addAtom(float x, float y, float z, float radius, float r, float g, float b) {
    if (atomCount == atomCapacity) {
        atomCapacity = atomCapacity == 0 ? 1 : atomCapacity * 2;
        atoms = (Atom*)realloc(atoms, atomCapacity * sizeof(Atom));
    }
    atoms[atomCount++] = (Atom){x, y, z, radius, r, g, b};
}

// Function to add a new atom at a specific screen position
void addAtomAtScreenPosition(int x, int y) {
    GLdouble modelview[16], projection[16];
    GLint viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLdouble winX = (double)x;
    GLdouble winY = (double)viewport[3] - (double)y - 1; // Adjust for OpenGL's coordinate system
    GLfloat winZ;
    glReadPixels(x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    GLdouble posX, posY, posZ;
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    addAtom((float)posX, (float)posY, (float)posZ, 0.1f, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
}

// Function to check if a point is inside an atom
int isPointInsideAtom(float x, float y, float z, const Atom* atom) {
    float dx = x - atom->x;
    float dy = y - atom->y;
    float dz = z - atom->z;
    return (dx * dx + dy * dy + dz * dz) <= (atom->radius * atom->radius);
}

// Function to select an atom
void selectAtom(int x, int y) {
    GLdouble modelview[16], projection[16];
    GLint viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLdouble winX = (double)x;
    GLdouble winY = (double)viewport[3] - (double)y;
    GLdouble winZ;
    glReadPixels(x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    GLdouble posX, posY, posZ;
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    selectedAtomIndex = -1;
    for (size_t i = 0; i < atomCount; ++i) {
        if (isPointInsideAtom(posX, posY, posZ, &atoms[i])) {
            selectedAtomIndex = i;
            break;
        }
    }
}

// Mouse callback function
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = 1;
            lastMouseX = x;
            lastMouseY = y;
            selectAtom(x, y); // Select atom on left click
            glutPostRedisplay();
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
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        addAtomAtScreenPosition(x, y); // Add a new atom at the right-click position
        glutPostRedisplay();
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
    renderAtoms();
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

void menu(int option); // Declare the menu function

void createMenu() {
    glutCreateMenu(menu);
    glutAddMenuEntry("New Atom", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(int option) {
    if (option == 1) {
        // Add a new atom at the center with random color
        addAtom(0.0f, 0.0f, 0.0f, 0.1f, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
        glutPostRedisplay();
    }
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Molecule Viewer");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape); // Register reshape callback
    glutMouseFunc(mouse); // Register mouse callback
    glutMotionFunc(motion); // Register motion callback
    createMenu(); // Create the menu
    initGL();
    glutMainLoop();
    free(atoms);
    return 0;
}
