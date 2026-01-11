#include <windows.h>
#include <GL/freeglut.h>
#include <math.h>

// Variabel Kontrol & Animasi
float angle = 0.0f;
float zoom = 1.0f;
bool unicornMode = false; 
float transition = 0.0f; // Untuk animasi transisi halus

// ================== FUNGSI PEMBANTU ==================
void drawCube(float x, float y, float z) {
    glPushMatrix();
    glScalef(x, y, z);
    glutSolidCube(1.0);
    glPopMatrix();
}

// Fungsi Panel Line dengan Efek Emission (Syarat B.4)
void drawPanelLine(float x, float y, float z) {
    GLfloat emissionRed[] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat no_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glPushMatrix();
    if (unicornMode) {
        glMaterialfv(GL_FRONT, GL_EMISSION, emissionRed);
        glColor3f(1.0, 0.0, 0.0);
    } else {
        glColor3f(0.2, 0.2, 0.2); // Warna panel tertutup
    }
    glScalef(x, y, z);
    glutSolidCube(1.0);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    glPopMatrix();
}

// ================== BAGIAN ANGGOTA TUBUH ==================

void drawHead() {
    // Helm
    glColor3f(1.0, 1.0, 1.0);
    drawCube(1.1, 1.1, 1.1);

    // Mata (Hijau ke Merah)
    glPushMatrix();
    glTranslatef(0.0, 0.2, 0.55);
    if (unicornMode) glColor3f(1.0, 0.0, 0.0); 
    else glColor3f(0.0, 1.0, 0.0);
    drawCube(0.6, 0.15, 0.1);
    glPopMatrix();

    // V-Fin (Normal: Kuning, Devil: Emas)
    if (unicornMode) glColor3f(1.0, 0.9, 0.0); 
    else glColor3f(1.0, 1.0, 0.0);

    for (int i = -1; i <= 1; i += 2) {
        if (i == 0) continue;
        glPushMatrix();
        glTranslatef(0.25 * i, 0.7, 0.3);
        // Devil gundam tanduknya lebih lebar (35 deg) dibanding angle (20 deg)
        glRotatef((20.0f + (15.0f * transition)) * -i, 0, 0, 1);
        drawCube(0.1, 0.8 + (0.1 * transition), 0.1);
        glPopMatrix();
    }
}

void drawArm(float side) {
    glPushMatrix();
    // Animasi bahu melebar saat devil mode
    glTranslatef(side * (1.4f + (0.1f * transition)), 0.5f + (0.1f * transition), 0.0f);

    // Bahu
    glColor3f(1.0, 1.0, 1.0);
    drawCube(1.0, 0.9, 1.0);

    // Lengan Atas
    glPushMatrix();
    glTranslatef(0.0f, -0.8f, 0.0f);
    glColor3f(0.7, 0.7, 0.7);
    drawCube(0.5, 1.4, 0.5);
    glPopMatrix();

    // Lengan Bawah & Panel Line
    glPushMatrix();
    glTranslatef(0.0f, -1.8f, 0.0f);
    glColor3f(1.0, 1.0, 1.0);
    drawCube(0.65, 1.2, 0.65);
    if(unicornMode){
        glTranslatef(0.0, 0.0, 0.35);
        drawPanelLine(0.4, 0.7, 0.05);
    }
    glPopMatrix();
    glPopMatrix();
}

void drawLeg(float side, bool isForward) {
    glPushMatrix();
    // Animasi Kaki Maju saat Devil Mode
    if(isForward && unicornMode) glTranslatef(0.0, 0.0, 0.5);

    // Paha
    glColor3f(0.6, 0.6, 0.6);
    drawCube(0.6, 1.5, 0.6);

    // Betis & Panel Merah
    glPushMatrix();
    glTranslatef(0.0, -1.3, 0.1);
    glColor3f(0.85, 0.85, 0.85);
    drawCube(0.8, 1.3, 0.7);
    if(unicornMode){
        glTranslatef(0.0, -0.2, 0.4);
        drawPanelLine(0.5, 0.8, 0.05);
    } else {
        // Detail Merah Angle Gundam
        glTranslatef(0.0, -0.3, 0.2);
        glColor3f(0.8, 0.1, 0.1);
        drawCube(0.5, 0.4, 0.3);
    }
    glPopMatrix();

    // Sepatu
    glPushMatrix();
    glTranslatef(0.0, -2.4, 0.3);
    glColor3f(0.8, 0.1, 0.1);
    drawCube(0.9, 0.4, 1.4);
    glPopMatrix();

    glPopMatrix();
}

void drawGundam() {
    // BADAN
    glPushMatrix();
    if (unicornMode) glColor3f(0.1, 0.3, 0.6); // Biru Torso Devil
    else glColor3f(0.8, 0.8, 0.8); // Abu Torso Angle
    drawCube(1.8, 2.2, 1.2);
    
    // Dada
    glTranslatef(0.0, 0.4, 0.6);
    if (unicornMode) drawPanelLine(1.4, 0.1, 0.1); // Panel menyala
    else { glColor3f(0.8, 0.1, 0.1); drawCube(1.4, 0.8, 0.3); } // Dada merah angle
    glPopMatrix();

    // KEPALA
    glPushMatrix();
    glTranslatef(0.0, 1.8, 0.0);
    drawHead();
    glPopMatrix();

    // TANGAN
    drawArm(1);
    drawArm(-1);

    // KAKI
    glPushMatrix();
    glTranslatef(0.7, -1.5, 0.0);
    drawLeg(1, true); // Kanan
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.7, -1.5, 0.0);
    drawLeg(-1, false); // Kiri
    glPopMatrix();
}

// ================== CORE FUNCTIONS ==================

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(7.0, 3.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Update Animasi Transisi
    if (unicornMode && transition < 1.0f) transition += 0.02f;
    else if (!unicornMode && transition > 0.0f) transition -= 0.02f;

    glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(zoom, zoom, zoom);

    // LANTAI & PODIUM
    glPushMatrix();
    glTranslatef(0.0, -5.5, 0.0);
    glColor3f(0.2, 0.2, 0.2); drawCube(15.0, 0.1, 15.0);
    glTranslatef(0.0, 0.4, 0.0);
    glColor3f(0.4, 0.4, 0.4); drawCube(4.0, 0.6, 4.0);
    glPopMatrix();

    drawGundam();

    angle += 0.01f;
    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': case 'W': zoom += 0.05f; break;
        case 's': case 'S': zoom -= 0.05f; break;
        case 'x': case 'X': unicornMode = !unicornMode; break;
    }
    if (zoom < 0.1f) zoom = 0.1f;
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
    glutInitWindowSize(900, 700);
    glutCreateWindow("Gundam Angle to Devil Transformation - Press X");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
