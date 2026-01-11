#include <windows.h>
#include <GL/freeglut.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ================== KONFIGURASI JUBAH ==================
const int W_PTS = 55;
const int H_PTS = 65;
struct Point { float x, y, z; };
Point cloak[W_PTS][H_PTS];

// ================== VARIABEL KONTROL ==================
float zoom = -45.0f;
float view_rotX = 15.0f, view_rotY = 45.0f;

// Posisi Shield - Rotasi 180 derajat agar menghadap ke luar
float sTX = 2.8f, sTY = -0.5f, sTZ = 1.2f;
float sRY = 180.0f; 

// Posisi Jubah
float cTY = 2.3f;   
float cTZ = -0.85f; 

// ================== FUNGSI PERISAI (SHIELD) ==================
void drawSide(float x1, float y1, float x2, float y2, float zFront, float zBack) {
    glBegin(GL_QUADS);
    glNormal3f(x1 - x2, y1 - y2, 0.0f); 
    glVertex3f(x1, y1, zFront);
    glVertex3f(x2, y2, zFront);
    glVertex3f(x2, y2, zBack);
    glVertex3f(x1, y1, zBack);
    glEnd();
}

void renderShield() {
    glPushMatrix();
    glTranslatef(sTX, sTY, sTZ);
    glRotatef(sRY, 0.0f, 1.0f, 0.0f); // Memutar perisai ke arah sebaliknya
    glScalef(1.2f, 1.2f, 1.2f);

    glDisable(GL_CULL_FACE); // Mencegah bagian belakang hilang

    float zBack = 0.0f, zMid = 0.1f, zFront = 0.15f, zCross = 0.20f;

    // Sisi Depan (Putih)
    glColor3f(0.95f, 0.95f, 0.95f);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex3f(-1.1f, 1.9f, zMid); glVertex3f(1.1f, 1.9f, zMid);
    glVertex3f(1.4f, 0.5f, zMid);  glVertex3f(0.8f, -2.2f, zMid);
    glVertex3f(-0.8f, -2.2f, zMid); glVertex3f(-1.4f, 0.5f, zMid);
    glEnd();

    // Sisi Belakang (Solid)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glBegin(GL_POLYGON);
    glVertex3f(-1.1f, 1.9f, zBack); glVertex3f(1.1f, 1.9f, zBack);
    glVertex3f(1.4f, 0.5f, zBack);  glVertex3f(0.8f, -2.2f, zBack);
    glVertex3f(-0.8f, -2.2f, zBack); glVertex3f(-1.4f, 0.5f, zBack);
    glEnd();

    // Sisi Tebal (Samping)
    glColor3f(0.7f, 0.7f, 0.7f);
    drawSide(-1.1f, 1.9f, 1.1f, 1.9f, zMid, zBack);
    drawSide(1.1f, 1.9f, 1.4f, 0.5f, zMid, zBack);
    drawSide(1.4f, 0.5f, 0.8f, -2.2f, zMid, zBack);
    drawSide(0.8f, -2.2f, -0.8f, -2.2f, zMid, zBack);
    drawSide(-0.8f, -2.2f, -1.4f, 0.5f, zMid, zBack);
    drawSide(-1.4f, 0.5f, -1.1f, 1.9f, zMid, zBack);

    // Bagian Merah
    glColor3f(0.9f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex3f(-0.8f, 1.6f, zFront); glVertex3f(0.8f, 1.6f, zFront);
    glVertex3f(1.1f, 0.5f, zFront);  glVertex3f(0.6f, -1.9f, zFront);
    glVertex3f(-0.6f, -1.9f, zFront); glVertex3f(-1.1f, 0.5f, zFront);
    glEnd();

    // Salib Kuning
    glColor3f(1.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(-0.15f, 0.6f, zCross); glVertex3f(0.15f, 0.6f, zCross);
    glVertex3f(0.15f, -1.6f, zCross); glVertex3f(-0.15f, -1.6f, zCross);
    glVertex3f(-0.6f, -0.4f, zCross); glVertex3f(0.6f, -0.4f, zCross);
    glVertex3f(0.6f, -0.7f, zCross); glVertex3f(-0.6f, -0.7f, zCross);
    glEnd();

    glEnable(GL_CULL_FACE);
    glPopMatrix();
}

// ================== LOGIKA JUBAH KUSUT ==================
void initCloak() {
    float rAtas = 1.9f, rBawah = 5.8f, tinggi = 10.0f;
    srand(time(0));
    for (int j = 0; j < H_PTS; j++) {
        float v = (float)j / (H_PTS - 1);
        float curR = rAtas * (1.0f - v) + rBawah * v;
        float posY = -v * tinggi;
        for (int i = 0; i < W_PTS; i++) {
            float u = (float)i / (W_PTS - 1);
            float angle = (u * 2.0f - 1.0f) * (M_PI / 1.7f) + M_PI; 
            
            // Tekstur kusut statis (Random Displacement)
            float wrinkle = ((rand() % 100) / 100.0f) * 0.2f;
            if (j < 5) wrinkle = 0; // Area bahu tetap rapi

            cloak[i][j].x = curR * sin(angle);
            cloak[i][j].y = posY;
            cloak[i][j].z = curR * cos(angle) + wrinkle;
        }
    }
}

// ================== DISPLAY & MAIN ==================
void drawGundam() {
    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix(); glScalef(2.5, 3.8, 1.6); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 2.8, 0); glColor3f(1,1,1); glutSolidCube(1.3); glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0, 0, zoom);
    glRotatef(view_rotX, 1, 0, 0);
    glRotatef(view_rotY, 0, 1, 0);

    GLfloat lightPos[] = { 10.0f, 10.0f, 15.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Render Jubah
    glPushMatrix();
    glTranslatef(0.0f, cTY, cTZ);
    glDisable(GL_CULL_FACE);
    glColor3f(0.12f, 0.12f, 0.25f);
    for (int j = 0; j < H_PTS - 1; j++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i < W_PTS; i++) {
            glNormal3f(0, 0, 1);
            glVertex3f(cloak[i][j].x, cloak[i][j].y, cloak[i][j].z);
            glVertex3f(cloak[i][j+1].x, cloak[i][j+1].y, cloak[i][j+1].z);
        }
        glEnd();
    }
    glEnable(GL_CULL_FACE);
    glPopMatrix();

    drawGundam();
    renderShield();

    glutSwapBuffers();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f); 
    initCloak();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(45.0, (float)w/h, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a': view_rotY -= 5.0f; break;
        case 'd': view_rotY += 5.0f; break;
        case 'w': view_rotX -= 5.0f; break;
        case 's': view_rotX += 5.0f; break;
        case 27: exit(0); break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Gundam - Reverse Shield 180");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

