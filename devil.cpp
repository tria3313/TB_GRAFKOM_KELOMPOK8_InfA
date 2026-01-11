#include <windows.h>
#include <GL/freeglut.h>

// Variabel Kontrol
float angle = 0.0f;
float zoom = 1.0f;
bool unicornMode = true; 

// ================== FUNGSI HELPER ==================
void drawCube(float x, float y, float z) {
    glPushMatrix();
    glScalef(x, y, z);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawPanelLine(float x, float y, float z) {
    GLfloat emissionRed[] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat no_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glPushMatrix();
    if (unicornMode) {
        glMaterialfv(GL_FRONT, GL_EMISSION, emissionRed);
        glColor3f(1.0, 0.0, 0.0);
    }
    glScalef(x, y, z);
    glutSolidCube(1.0);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    glPopMatrix();
}

// ================== BAGIAN TANGAN (HIERARKI REVISI) ==================
void drawArm(float side) {
    glPushMatrix();
    // Posisi awal pundak relatif terhadap badan
    glTranslatef(side * 1.5f, 0.6f, 0.0f); 

    // 1. BAHU (SHOULDER - Besar)
    glColor3f(1.0, 1.0, 1.0);
    drawCube(1.0, 0.9, 1.0);

    // 2. LENGAN ATAS (UPPER ARM - Panjang)
    glPushMatrix();
    glTranslatef(0.0f, -0.9f, 0.0f);
    glColor3f(0.85, 0.85, 0.85);
    drawCube(0.6, 1.4, 0.6);
    glPopMatrix();

    // 3. SIKU (ELBOW - Kecil)
    glPushMatrix();
    glTranslatef(0.0f, -1.7f, 0.1f);
    glColor3f(0.7, 0.7, 0.7);
    drawCube(0.4, 0.3, 0.4);
    glPopMatrix();

    // 4. LENGAN BAWAH (FOREARM - Tebal)
    glPushMatrix();
    glTranslatef(0.0f, -2.5f, 0.0f);
    glColor3f(0.9, 0.9, 0.9);
    drawCube(0.7, 1.6, 0.8);
    // Psycho Frame Lengan
    glTranslatef(0.0f, 0.0f, 0.45f);
    drawPanelLine(0.4, 0.8, 0.05);
    glPopMatrix();

    // 5. TANGAN (HAND - Kotak Kecil)
    glPushMatrix();
    glTranslatef(0.0f, -3.5f, 0.0f);
    glColor3f(0.5, 0.5, 0.5); // Warna sendi tangan
    drawCube(0.4, 0.4, 0.4);
    glPopMatrix();

    glPopMatrix(); // Tutup drawArm
}

// ================== BAGIAN TUBUH LAINNYA ==================
void drawHead() {
    glColor3f(1.0, 1.0, 1.0);
    drawCube(1.0, 1.1, 1.0);
    // Mata Menyala
    glPushMatrix();
    glTranslatef(0.0, 0.2, 0.55);
    if (unicornMode) glColor3f(1.0, 0.0, 0.0); else glColor3f(0.0, 1.0, 0.0);
    drawCube(0.6, 0.15, 0.1);
    glPopMatrix();
    // Tanduk V-Fin
    glColor3f(1.0, 0.9, 0.0);
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(0.15 * i, 0.7, 0.3);
        glRotatef(35 * i, 0, 0, 1);
        drawCube(0.1, 0.9, 0.05);
        glPopMatrix();
    }
}

void drawLeg(float side) {
    glPushMatrix();
    // Paha
    glColor3f(0.9, 0.9, 0.9);
    drawCube(0.7, 1.6, 0.7);
    // Panel Betis
    glPushMatrix();
    glTranslatef(0.0, -1.8, 0.1);
    glColor3f(0.85, 0.85, 0.85);
    drawCube(0.9, 1.4, 0.8);
    // Psycho frame betis
    glTranslatef(0.0, 0.0, 0.45);
    drawPanelLine(0.5, 0.9, 0.05);
    glPopMatrix();
    // Sepatu
    glPushMatrix();
    glTranslatef(0.0, -2.8, 0.3);
    glColor3f(0.8, 0.1, 0.1);
    drawCube(0.9, 0.4, 1.5);
    glPopMatrix();
    glPopMatrix();
}

void drawGundam() {
    // TORSO & PANEL DADA
    glPushMatrix();
    glColor3f(0.1, 0.3, 0.6);
    drawCube(1.8, 1.8, 1.2);
    // Panel Dada
    glPushMatrix();
    glTranslatef(0.0, -0.4, 0.65);
    drawPanelLine(1.4, 0.08, 0.05);
    glPopMatrix();
    glPopMatrix();

    // KEPALA
    glPushMatrix();
    glTranslatef(0.0, 1.7, 0.0);
    drawHead();
    glPopMatrix();

    // TANGAN (REVISI)
    drawArm(1);  // Kanan
    drawArm(-1); // Kiri

    // KAKI (POSE HERO)
    glPushMatrix();
    glTranslatef(0.8, -2.0, 0.5); // Kanan Maju
    drawLeg(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.8, -2.0, 0.0); // Kiri Normal
    drawLeg(-1);
    glPopMatrix();
}

// ================== SISTEM CORE ==================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(6.0, 3.0, 12.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(zoom, zoom, zoom);

    // Lantai & Podium
    glPushMatrix();
    glTranslatef(0.0, -6.5, 0.0);
    glColor3f(0.2, 0.2, 0.2); drawCube(15.0, 0.1, 15.0);
    glTranslatef(0.0, 0.4, 0.0);
    glColor3f(0.4, 0.4, 0.4); drawCube(5.0, 0.8, 5.0);
    glPopMatrix();

    drawGundam();

    angle += 0.01f;
    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') zoom += 0.05f;
    if (key == 's' || key == 'S') zoom -= 0.05f;
    if (key == 'm' || key == 'M') unicornMode = !unicornMode;
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    GLfloat light_pos[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Gundam Unicorn - Final Revised");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
