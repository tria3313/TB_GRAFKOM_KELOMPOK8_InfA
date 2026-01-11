#include <windows.h>
#include <GL/freeglut.h>
#include <math.h>

float angle = 0.0f;
float zoom = 1.0f;
bool unicornMode = false; 
float transition = 0.0f; 

// --- FUNGSI BARU: Mengatur waktu mekar per bagian ---
// start & end adalah rentang waktu dalam 0.0 - 1.0
float getPartTransition(float start, float end) {
    if (transition < start) return 0.0f;
    if (transition > end) return 1.0f;
    return (transition - start) / (end - start);
}

void drawCube(float x, float y, float z) {
    glPushMatrix();
    glScalef(x, y, z);
    glutSolidCube(1.0);
    glPopMatrix();
}

// Fungsi menggambar Armor yang bisa menonjol (Pop-out)
void drawArmorPlate(float x, float y, float z, float p, bool isRed = true) {
    glPushMatrix();
    // EFEK MEKAR: Geser ke luar (sumbu Z) dan sedikit membesar berdasarkan p
    glTranslatef(0.0f, 0.0f, p * 0.25f); 
    
    if (unicornMode) {
        // Efek Menyala Merah (Psycho Frame)
        GLfloat glow[] = { p, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, glow);
        if(isRed) glColor3f(1.0f, 0.2f * (1.0f - p), 0.2f * (1.0f - p));
        else glColor3f(1.0, 1.0, 1.0);
    } else {
        glColor3f(1.0, 1.0, 1.0);
    }
    
    drawCube(x, y, z);
    
    GLfloat no_glow[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_glow);
    glPopMatrix();
}

void drawHead() {
    float p = getPartTransition(0.7f, 1.0f); // Kepala mekar terakhir (70%-100%)
    
    glColor3f(1.0, 1.0, 1.0);
    drawCube(1.1, 1.1, 1.1); // Base Kepala

    // Mata berubah warna
    glPushMatrix();
    glTranslatef(0.0, 0.2, 0.55);
    glColor3f(p, 1.0f - p, 0.0);
    drawCube(0.6, 0.15, 0.1);
    glPopMatrix();

    // Tanduk Mekar (V-Fin)
    for (int i = -1; i <= 1; i += 2) {
        if (i == 0) continue;
        glPushMatrix();
        glTranslatef(0.25 * i, 0.7, 0.3);
        glRotatef((20.0f + (25.0f * p)) * -i, 0, 0, 1);
        glColor3f(1.0, 1.0 - (0.1f * p), 0.0);
        drawCube(0.1, 0.8 + (0.2f * p), 0.1);
        glPopMatrix();
    }
}

void drawArm(float side) {
    float p = getPartTransition(0.4f, 0.8f); // Tangan mekar (40%-80%)
    glPushMatrix();
    glTranslatef(side * (1.4f + (0.2f * p)), 0.5f, 0.0f);

    // Bahu Mekar
    drawArmorPlate(1.0, 0.9, 1.0, p, false);

    // Lengan
    glPushMatrix();
    glTranslatef(0.0f, -0.9f, 0.0f);
    glColor3f(0.7, 0.7, 0.7);
    drawCube(0.5, 1.4, 0.5);
    // Panel Lengan Pop-out
    glTranslatef(0.0, -0.2, 0.1);
    drawArmorPlate(0.6, 0.8, 0.6, p);
    glPopMatrix();
    glPopMatrix();
}

void drawLeg(float side, bool isForward) {
    float p = getPartTransition(0.0f, 0.5f); // Kaki mekar duluan (0%-50%)
    
    glPushMatrix();
    if(isForward && unicornMode) glTranslatef(0.0, 0.0, 0.5f * p);

    // Paha
    glColor3f(0.6, 0.6, 0.6);
    drawCube(0.6, 1.5, 0.6);

    // Betis Pop-out
    glPushMatrix();
    glTranslatef(0.0, -1.3, 0.1);
    drawArmorPlate(0.8, 1.3, 0.7, p);
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
    float pBody = getPartTransition(0.3f, 0.7f); // Badan (30%-70%)
    
    // TORSO
    glPushMatrix();
    glColor3f(0.8f - (0.7f * pBody), 0.8f - (0.5f * pBody), 0.8f - (0.2f * pBody));
    drawCube(1.8, 2.2, 1.2);
    // Dada Pop-out
    glTranslatef(0.0, 0.4, 0.5);
    drawArmorPlate(1.4, 0.8, 0.4, pBody);
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
    gluLookAt(10.0, 5.0, 18.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Animasi Transisi Halus & Sequential
    if (unicornMode) {
        if (transition < 1.0f) transition += 0.008f; 
    } else {
        if (transition > 0.0f) transition -= 0.008f;
    }

    glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(zoom, zoom, zoom);

    // Lantai
    glPushMatrix();
    glTranslatef(0.0, -5.5, 0.0);
    glColor3f(0.2, 0.2, 0.2); drawCube(20.0, 0.1, 20.0);
    glPopMatrix();

    drawGundam();

    angle += 0.1f;
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
    GLfloat light_pos[] = { 5.0, 5.0, 10.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glClearColor(0.0, 0.0, 0.1, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 900.0/700.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 700);
    glutCreateWindow("Gundam Sequential Armor Pop-Out");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
