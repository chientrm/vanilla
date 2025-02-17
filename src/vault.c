#include <GL/glut.h>

float angleX = 0.0f;
float angleY = 0.0f;
float zoom = -5.0f;
int lastMouseX, lastMouseY;
int isDragging = 0;

typedef struct {
    float vertices[16][3];
    int edges[24][2];
} MajorVaultProtein;

MajorVaultProtein mvp = {
    {
        { -0.05f, -0.05f, -0.05f }, {  0.05f, -0.05f, -0.05f },
        {  0.05f,  0.05f, -0.05f }, { -0.05f,  0.05f, -0.05f },
        { -0.05f, -0.05f,  0.05f }, {  0.05f, -0.05f,  0.05f },
        {  0.05f,  0.05f,  0.05f }, { -0.05f,  0.05f,  0.05f },
        { -0.025f, -0.025f, -0.025f }, {  0.025f, -0.025f, -0.025f },
        {  0.025f,  0.025f, -0.025f }, { -0.025f,  0.025f, -0.025f },
        { -0.025f, -0.025f,  0.025f }, {  0.025f, -0.025f,  0.025f },
        {  0.025f,  0.025f,  0.025f }, { -0.025f,  0.025f,  0.025f }
    },
    {
        { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
        { 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 },
        { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 },
        { 8, 9 }, { 9, 10 }, { 10, 11 }, { 11, 8 },
        { 12, 13 }, { 13, 14 }, { 14, 15 }, { 15, 12 },
        { 8, 12 }, { 9, 13 }, { 10, 14 }, { 11, 15 }
    }
};

void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // Ambient light
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
}

void drawMVP() {
    glBegin(GL_QUADS);
    // Front face
    glVertex3fv(mvp.vertices[0]);
    glVertex3fv(mvp.vertices[1]);
    glVertex3fv(mvp.vertices[2]);
    glVertex3fv(mvp.vertices[3]);
    // Back face
    glVertex3fv(mvp.vertices[4]);
    glVertex3fv(mvp.vertices[5]);
    glVertex3fv(mvp.vertices[6]);
    glVertex3fv(mvp.vertices[7]);
    // Left face
    glVertex3fv(mvp.vertices[0]);
    glVertex3fv(mvp.vertices[3]);
    glVertex3fv(mvp.vertices[7]);
    glVertex3fv(mvp.vertices[4]);
    // Right face
    glVertex3fv(mvp.vertices[1]);
    glVertex3fv(mvp.vertices[2]);
    glVertex3fv(mvp.vertices[6]);
    glVertex3fv(mvp.vertices[5]);
    // Top face
    glVertex3fv(mvp.vertices[3]);
    glVertex3fv(mvp.vertices[2]);
    glVertex3fv(mvp.vertices[6]);
    glVertex3fv(mvp.vertices[7]);
    // Bottom face
    glVertex3fv(mvp.vertices[0]);
    glVertex3fv(mvp.vertices[1]);
    glVertex3fv(mvp.vertices[5]);
    glVertex3fv(mvp.vertices[4]);
    glEnd();
}

void drawVaultCell() {
    for (int j = 0; j < 8; j++) {
        glPushMatrix();
        glTranslatef(0.0f, j * 0.1f, 0.0f); // Stack rings neatly together
        for (int i = 0; i < 360; i += (360 / 39)) {
            glPushMatrix();
            glRotatef(i, 0.0f, 1.0f, 0.0f);
            glTranslatef(1.0f, 0.0f, 0.0f);
            drawMVP();
            glPopMatrix();
        }
        glPopMatrix();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    // Draw the vault cell composed of Major Vault Proteins
    glColor3f(0.5f, 0.5f, 0.5f);
    drawVaultCell();

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
