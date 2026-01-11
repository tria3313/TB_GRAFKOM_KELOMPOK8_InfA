#include <windows.h>
#include <GL/freeglut.h>
#include <math.h>

float angle = 0.0f;
float zoom = 1.0f; // Variabel baru untuk fitur Scale (Syarat B.4)

// ================== FUNGSI PEMBANTU ==================
void drawCube(float x, float y, float z) {
    glPushMatrix();
    glScalef(x, y, z);
    glutSolidCube(1.0);
    glPopMatrix();
}

// ================== BAGIAN GUNDAM ==================
void drawHead() {
    glColor3f(1.0, 1.0, 1.0);
    drawCube(1.1, 1.1, 1.1); // HELM

    glPushMatrix();
    glTranslatef(0.0, 0.2, 0.55);
    glColor3f(0.0, 1.0, 0.0);
    drawCube(0.6, 0.15, 0.1); // MATA
    glPopMatrix();

    glColor3f(1.0, 1.0, 0.0);
    glPushMatrix();
    glTranslatef(-0.25, 0.7, 0.3);
    glRotatef(20, 0, 0, 1);
    drawCube(0.1, 0.8, 0.1); // TANDUK KIRI
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.25, 0.7, 0.3);
    glRotatef(-20, 0, 0, 1);
    drawCube(0.1, 0.8, 0.1); // TANDUK KANAN
    glPopMatrix();
}

void drawCalfArmor() {
    glPushMatrix();
    glTranslatef(0.0f, -0.8f, 0.1f);
    glColor3f(0.85f, 0.85f, 0.85f);
    drawCube(0.7f, 1.2f, 0.6f);
    
    glTranslatef(0.0f, -0.3f, 0.2f);
    glColor3f(0.8f, 0.1f, 0.1f);
    drawCube(0.5f, 0.4f, 0.3f);
    glPopMatrix();
}

void drawFoot() {
    glColor3f(0.9, 0.9, 0.9);
    drawCube(0.8, 0.3, 1.2);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.6);
    glColor3f(0.8, 0.1, 0.1);
    drawCube(0.6, 0.25, 0.4);
    glPopMatrix();
}

void drawGundam() {
    glPushMatrix();
    glColor3f(0.8, 0.8, 0.8);
    drawCube(1.8, 2.2, 1.2); // BADAN
    
    glTranslatef(0.0, 0.4, 0.6);
    glColor3f(0.8, 0.1, 0.1);
    drawCube(1.4, 0.8, 0.3); // DADA
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 1.8, 0.0);
    drawHead();
    glPopMatrix();

    // TANGAN KIRI & KANAN
    for (int i = -1; i <= 1; i += 2) {
        if (i == 0) continue;
        glPushMatrix();
        glTranslatef(1.4 * i, 0.5, 0.0);
        glColor3f(0.7, 0.7, 0.7);
        drawCube(0.5, 1.8, 0.5);
        glTranslatef(0.0, -1.2, 0.0);
        glColor3f(1.0, 1.0, 1.0);
        drawCube(0.6, 0.8, 0.6);
        glPopMatrix();
    }

    // KAKI KIRI & KANAN
    for (int i = -1; i <= 1; i += 2) {
        if (i == 0) continue;
        glPushMatrix();
        glTranslatef(0.6 * i, -1.5, 0.0);
        glColor3f(0.6, 0.6, 0.6);
        drawCube(0.6, 1.5, 0.6);
        drawCalfArmor();
        glTranslatef(0.0, -1.6, 0.3);
        drawFoot();
        glPopMatrix();
    }
}

void drawScene() {
    glPushMatrix();
    glTranslatef(0.0, -3.8, 0.0);
    glColor3f(0.2, 0.2, 0.2);
    drawCube(15.0, 0.1, 15.0); // LANTAI
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, -3.5, 0.0);
    glColor3f(0.4, 0.4, 0.4);
    drawCube(4.0, 0.5, 4.0); // PODIUM
    glPopMatrix();
}

// ================== CORE FUNCTIONS ==================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 2.0, 12.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // IMPLEMENTASI TRANSFORMASI (Syarat B.4)
    glRotatef(angle, 0.0, 1.0, 0.0); // Rotate
    glScalef(zoom, zoom, zoom);      // Scale (Zoom In/Out)

    drawScene();
    drawGundam();

    angle += 0.01f;
    glutSwapBuffers();
    glutPostRedisplay();
}

// Fungsi Keyboard untuk Zoom (Syarat B.3 Opsi B)
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': case 'W':
            zoom += 0.05f; // Memperbesar
            break;
        case 's': case 'S':
            zoom -= 0.05f; // Memperkecil
            break;
    }
    if (zoom < 0.1f) zoom = 0.1f; // Batas minimum agar objek tidak hilang
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);   
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    GLfloat light_pos[] = { 5.0, 5.0, 5.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glClearColor(0.05, 0.05, 0.1, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0/600.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tugas Besar Grafkom - Pameran Gundam");
    init();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard); // Mendaftarkan fungsi keyboard
    
    glutMainLoop();
    return 0;
}
