#include <windows.h>
#include <GL/freeglut.h>
#include <math.h>

// Variabel Kontrol & Animasi
float angle = 0.0f;
float zoom = 1.0f;
bool unicornMode = false; 
float transition = 0.0f; 

// ================== FUNGSI PEMBANTU ==================
void drawCube(float x, float y, float z) {
    glPushMatrix();
    glScalef(x, y, z);
    glutSolidCube(1.0);
    glPopMatrix();
}

// --- FUNGSI NAMA GUNDAM 3D & BERCAHAYA (PRESISI TENGAH) ---
void drawGundamName() {
    glPushMatrix();
    
    // POSISI CENTER: 
    // -5.3f adalah titik hitung agar spasi antara LUCIFER dan GUNDAM tepat di sumbu 0
    // 5.5f memberikan jarak aman di atas tanduk
    glTranslatef(-5.3f, 5.5f, 0.0f); 
    
    glScalef(0.012f, 0.012f, 0.012f); 

    // Efek Cahaya (Glow Merah)
    GLfloat emissionRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, emissionRed);
    glColor3f(1.0f, 0.0f, 0.0f); 

    // EFEK DIMENSI 3D (TEBAL)
    for (float i = 0; i < 5.0f; i += 0.5f) {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, i * 0.1f); 
        glLineWidth(6.0f); // Dibuat lebih tebal lagi agar sangar

        const char* name = "LUCIFER GUNDAM";
        for (const char* c = name; *c != '\0'; c++) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
    }

    // Reset Emission
    GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    glPopMatrix();
}

// Fungsi Panel Line dengan Efek Emission
void drawPanelLine(float x, float y, float z) {
    GLfloat emissionRed[] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat no_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glPushMatrix();
    if (unicornMode) {
        glMaterialfv(GL_FRONT, GL_EMISSION, emissionRed);
        glColor3f(1.0, 0.0, 0.0);
    } else {
        glColor3f(0.2, 0.2, 0.2); 
    }
    glScalef(x, y, z);
    glutSolidCube(1.0);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    glPopMatrix();
}

// ================== BAGIAN ANGGOTA TUBUH ==================

void drawHead() {
    glColor3f(1.0, 1.0, 1.0);
    drawCube(1.1, 1.1, 1.1);

    glPushMatrix();
    glTranslatef(0.0, 0.2, 0.55);
    if (unicornMode) glColor3f(1.0, 0.0, 0.0); 
    else glColor3f(0.0, 1.0, 0.0);
    drawCube(0.6, 0.15, 0.1);
    glPopMatrix();

    if (unicornMode) glColor3f(1.0, 0.9, 0.0); 
    else glColor3f(1.0, 1.0, 0.0);

    for (int i = -1; i <= 1; i += 2) {
        if (i == 0) continue;
        glPushMatrix();
        glTranslatef(0.25 * i, 0.7, 0.3);
        glRotatef((20.0f + (15.0f * transition)) * -i, 0, 0, 1);
        drawCube(0.1, 0.8 + (0.1 * transition), 0.1);
        glPopMatrix();
    }
}

void drawArm(float side) {
    glPushMatrix();
    glTranslatef(side * (1.4f + (0.1f * transition)), 0.5f + (0.1f * transition), 0.0f);
    glColor3f(1.0, 1.0, 1.0);
    drawCube(1.0, 0.9, 1.0);

    glPushMatrix();
    glTranslatef(0.0f, -0.8f, 0.0f);
    glColor3f(0.7, 0.7, 0.7);
    drawCube(0.5, 1.4, 0.5);
    glPopMatrix();

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
    if(isForward && unicornMode) glTranslatef(0.0, 0.0, 0.5);
    glColor3f(0.6, 0.6, 0.6);
    drawCube(0.6, 1.5, 0.6);

    glPushMatrix();
    glTranslatef(0.0, -1.3, 0.1);
    glColor3f(0.85, 0.85, 0.85);
    drawCube(0.8, 1.3, 0.7);
    if(unicornMode){
        glTranslatef(0.0, -0.2, 0.4);
        drawPanelLine(0.5, 0.8, 0.05);
    } else {
        glTranslatef(0.0, -0.3, 0.2);
        glColor3f(0.8, 0.1, 0.1);
        drawCube(0.5, 0.4, 0.3);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, -2.4, 0.3);
    glColor3f(0.8, 0.1, 0.1);
    drawCube(0.9, 0.4, 1.4);
    glPopMatrix();
    glPopMatrix();
}

void drawGundam() {
    glPushMatrix();
    if (unicornMode) glColor3f(0.1, 0.3, 0.6); 
    else glColor3f(0.8, 0.8, 0.8); 
    drawCube(1.8, 2.2, 1.2);
    
    glTranslatef(0.0, 0.4, 0.6);
    if (unicornMode) drawPanelLine(1.4, 0.1, 0.1); 
    else { glColor3f(0.8, 0.1, 0.1); drawCube(1.4, 0.8, 0.3); } 
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 1.8, 0.0);
    drawHead();
    glPopMatrix();

    drawArm(1);
    drawArm(-1);

    glPushMatrix(); glTranslatef(0.7, -1.5, 0.0); drawLeg(1, true); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.7, -1.5, 0.0); drawLeg(-1, false); glPopMatrix();
}

// ================== CORE FUNCTIONS ==================

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Kamera diatur agar Gundam dan Teks terlihat pas
    gluLookAt(10.0, 5.0, 18.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    if (unicornMode && transition < 1.0f) transition += 0.02f;
    else if (!unicornMode && transition > 0.0f) transition -= 0.02f;

    glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(zoom, zoom, zoom);

    // LANTAI
    glPushMatrix();
    glTranslatef(0.0, -5.5, 0.0);
    glColor3f(0.1, 0.1, 0.1); drawCube(20.0, 0.1, 20.0);
    glPopMatrix();

    drawGundam();
    drawGundamName(); // Panggil Teks yang sudah di-center

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
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    GLfloat light_pos[] = { 5.0, 8.0, 10.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glClearColor(0.0, 0.0, 0.02, 1.0); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 900.0/700.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 700);
    glutCreateWindow("LUCIFER GUNDAM - Center Aligned Edition");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
