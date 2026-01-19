#include <windows.h>
#include <GL/freeglut.h>
#include <math.h>
#include <cstdlib>
#include <ctime>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
//cipa
float rotationAngle = 0.0f;
float zoom = 1.0f;
bool unicornMode = false;
float transition = 0.0f;

float doorSlide = 0.0f;
bool doorOpening = false;
bool doorClosing = false;
//juan
bool lightsOn = true;
float spotlightCutoff = 30.0f;
//fathir
float gundamScale = 1.0f;
const float MIN_SCALE = 0.5f;
const float MAX_SCALE = 1.4f;
//fathir
float cloakOffsetY = 1.25f;
float cloakOffsetZ = -0.3f; //patokan jubah bagian atas

const int WIDTH_POINTS = 25;//ukuran cloak
const int HEIGHT_POINTS = 35;
float timeStep = 0.0f;
//
struct Point 
{
    float x, y, z;
    float baseX, baseY, baseZ;
};

Point cloak[WIDTH_POINTS][HEIGHT_POINTS];
bool damageMap[WIDTH_POINTS][HEIGHT_POINTS];


void drawSide(float x1, float y1, float x2, float y2, float zFront, float zBack) 
{
    glBegin(GL_QUADS);
    glNormal3f(y1 - y2, x2 - x1, 0.0f);
    glVertex3f(x1, y1, zFront);
    glVertex3f(x2, y2, zFront);
    glVertex3f(x2, y2, zBack);
    glVertex3f(x1, y1, zBack);
    glEnd();
}
//juan

/* Fungsi untuk menggambar perisai (shield)  */
void drawShield() 
{
    glPushMatrix();
    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    float zBack = -0.15f;
    float zMid = 0.1f;
    float zFront = 0.25f;
    float zCross = 0.35f;

    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_POLYGON);
    glNormal3f(0, 0, 1);
    glVertex3f(-1.1f, 1.9f, zMid);
    glVertex3f(1.1f, 1.9f, zMid);
    glVertex3f(1.4f, 0.5f, zMid);
    glVertex3f(0.8f, -2.2f, zMid);
    glVertex3f(-0.8f, -2.2f, zMid);
    glVertex3f(-1.4f, 0.5f, zMid);
    glEnd();

    glColor3f(0.7f, 0.7f, 0.7f);
    drawSide(-1.1f, 1.9f, 1.1f, 1.9f, zMid, zBack);
    drawSide(1.1f, 1.9f, 1.4f, 0.5f, zMid, zBack);
    drawSide(1.4f, 0.5f, 0.8f, -2.2f, zMid, zBack);
    drawSide(0.8f, -2.2f, -0.8f, -2.2f, zMid, zBack);
    drawSide(-0.8f, -2.2f, -1.4f, 0.5f, zMid, zBack);
    drawSide(-1.4f, 0.5f, -1.1f, 1.9f, zMid, zBack);

    glColor3f(0.8f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glNormal3f(0, 0, 1);
    glVertex3f(-0.8f, 1.6f, zFront);
    glVertex3f(0.8f, 1.6f, zFront);
    glVertex3f(1.1f, 0.5f, zFront);
    glVertex3f(0.6f, -1.9f, zFront);
    glVertex3f(-0.6f, -1.9f, zFront);
    glVertex3f(-1.1f, 0.5f, zFront);
    glEnd();

    glColor3f(0.5f, 0.0f, 0.0f);
    drawSide(-0.8f, 1.6f, 0.8f, 1.6f, zFront, zMid);
    drawSide(0.8f, 1.6f, 1.1f, 0.5f, zFront, zMid);
    drawSide(1.1f, 0.5f, 0.6f, -1.9f, zFront, zMid);
    drawSide(0.6f, -1.9f, -0.6f, -1.9f, zFront, zMid);
    drawSide(-0.6f, -1.9f, -1.1f, 0.5f, zFront, zMid);
    drawSide(-1.1f, 0.5f, -0.8f, 1.6f, zFront, zMid);

    glColor3f(1.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-0.15f, 0.6f, zCross);
    glVertex3f(0.15f, 0.6f, zCross);
    glVertex3f(0.15f, -1.6f, zCross);
    glVertex3f(-0.15f, -1.6f, zCross);
    glVertex3f(-0.6f, -0.4f, zCross);
    glVertex3f(0.6f, -0.4f, zCross);
    glVertex3f(0.6f, -0.7f, zCross);
    glVertex3f(-0.6f, -0.7f, zCross);
    glEnd();

    glColor3f(0.7f, 0.5f, 0.0f);
    drawSide(-0.15f, 0.6f, 0.15f, 0.6f, zCross, zFront);
    drawSide(-0.15f, -1.6f, 0.15f, -1.6f, zCross, zFront);
    drawSide(-0.6f, -0.4f, -0.6f, -0.7f, zCross, zFront);
    drawSide(0.6f, -0.4f, 0.6f, -0.7f, zCross, zFront);

    glPopMatrix();
}
//fathir
/* Fungsi untuk menggambar jubah yang bisa bergerak */
void drawCloak() 
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();

    glTranslatef(0.0f, cloakOffsetY, cloakOffsetZ);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.8f, 0.8f, 0.8f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);

    if (unicornMode)
    {
        glColor3f(0.3f, 0.0f, 0.0f);
    }
    else
    {
        glColor3f(0.1, 0.3, 0.6f);
    }

    glBegin(GL_QUADS);
    for (int j = 0; j < HEIGHT_POINTS - 1; j++) 
    {
        for (int i = 0; i < WIDTH_POINTS - 1; i++) 
        {
            if (damageMap[i][j] && damageMap[i + 1][j] &&
                damageMap[i][j + 1] && damageMap[i + 1][j + 1]) 
            {
                glNormal3f(0, 0, 1);
                glVertex3f(cloak[i][j].x, cloak[i][j].y, cloak[i][j].z);
                glVertex3f(cloak[i + 1][j].x, cloak[i + 1][j].y, cloak[i + 1][j].z);
                glVertex3f(cloak[i + 1][j + 1].x, cloak[i + 1][j + 1].y, cloak[i + 1][j + 1].z);
                glVertex3f(cloak[i][j + 1].x, cloak[i][j + 1].y, cloak[i][j + 1].z);
            }
        }
    }
    glEnd();

    glPopMatrix();
    glPopAttrib();
}
//cipa
/* Fungsi helper untuk menggambar kubus dengan ukuran custom
   Parameter x, y, z adalah skala untuk masing-masing sumbu */
void drawCube(float x, float y, float z) 
{
    glPushMatrix();
    glScalef(x, y, z);
    glutSolidCube(1.0);
    glPopMatrix();
}

/* Fungsi untuk menggambar kepala Gundam
   Termasuk antena yang bisa berubah saat mode Unicorn */
void drawHead() 
{
    glColor3f(1.0, 1.0, 1.0);
    drawCube(1.1, 1.1, 1.1);
    
    glPushMatrix();
    glTranslatef(0.0, 0.2, 0.55);
    if (unicornMode)
    {
        glColor3f(1.0, 0.0, 0.0);
    }
    else
    {
        glColor3f(0.0, 1.0, 0.0);
    }
    drawCube(0.6, 0.15, 0.1);
    glPopMatrix();

    glColor3f(1.0, 1.0, 0.0);
    for (int i = -1; i <= 1; i += 2) 
    {
        if (i == 0)
        {
            continue;
        }
        glPushMatrix();
        glTranslatef(0.25 * i, 0.7, 0.3);
        glRotatef((20.0f + (15.0f * transition)) * -i, 0, 0, 1);
        drawCube(0.1, 0.8 + (0.1 * transition), 0.1);
        glPopMatrix();
    }
}

/* Fungsi untuk menggambar lengan Gundam
    1 untuk kanan, -1 untuk kiri*/
void drawArm(float side) 
{
    glPushMatrix();
    glTranslatef(side * (1.4f + (0.1f * transition)), 0.5f, 0.0f);
    glColor3f(1.0, 1.0, 1.0);
    drawCube(1.0, 1.5, 1.0);

    glPushMatrix();
    glTranslatef(0.0f, -1.2f, 0.0f);
    drawCube(0.65, 1.2, 0.65);
    
    if (side == -1) 
    {
        glPushMatrix();
        glTranslatef(-0.7f, -0.2f, 0.3f);
        glRotatef(-90, 0, 1, 0);
        glRotatef(10, 1, 0, 0);
        glScalef(0.8f, 0.8f, 0.8f);
        drawShield();
        glPopMatrix();
    }
    glPopMatrix();
    glPopMatrix();
}

/* Fungsi untuk menggambar kaki Gundam*/
void drawLeg(float side) 
{
    glPushMatrix();
    glColor3f(0.6, 0.6, 0.6);
    drawCube(0.6, 1.5, 0.6);
    
    glPushMatrix();
    glTranslatef(0.0, -1.3, 0.1);
    glColor3f(0.85, 0.85, 0.85);
    drawCube(0.8, 1.3, 0.7);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0, -2.4, 0.3);
    if (unicornMode)
    {
        glColor3f(0.3f, 0.0f, 0.0f);
    }
    else
    {
        glColor3f(0.1, 0.3, 0.6f);
    }
    drawCube(0.9, 0.7, 1.4);
    glPopMatrix();
    glPopMatrix();
}

/* Fungsi utama untuk menggambar seluruh Gundam */
void drawGundam() 
{
    glPushMatrix();
    glTranslatef(0.0f, -4.5f * gundamScale, 0.0f);
    glScalef(gundamScale, gundamScale, gundamScale);
    glTranslatef(0.0f, 4.5f, 0.0f);

    drawCloak();

    if (unicornMode)
    {
        glColor3f(0.1, 0.3, 0.6);
    }
    else
    {
        glColor3f(0.8, 0.8, 0.8);
    }
    drawCube(1.8, 2.2, 1.2);

    glPushMatrix();
    glTranslatef(0.0, 1.8, 0.0);
    drawHead();
    glPopMatrix();

    drawArm(1);
    drawArm(-1);

    glPushMatrix();
    glTranslatef(0.7, -1.5, 0.0);
    drawLeg(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.7, -1.5, 0.0);
    drawLeg(-1);
    glPopMatrix();

    glPopMatrix();
}
//juan
/* Fungsi untuk menggambar lampu spotlight di showcase
   Membuat cylinder sebagai body lampu dan sphere sebagai bohlam */
void drawSpotlight(float x, float y, float z) 
{
    glPushMatrix();
    glTranslatef(x, y, z);

    float dirX = -x;
    float dirY = -y;
    float dirZ = -z;
    float len = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
    dirX /= len;
    dirY /= len;
    dirZ /= len;

    float angleY = atan2(dirX, dirZ) * 180.0f / M_PI;
    float angleX = -atan2(dirY, sqrt(dirX * dirX + dirZ * dirZ)) * 180.0f / M_PI;

    glRotatef(angleY, 0, 1, 0);
    glRotatef(angleX, 1, 0, 0);

    glDisable(GL_LIGHTING);

    glColor3f(0.2f, 0.2f, 0.2f);
    GLUquadric *quad = gluNewQuadric();
    gluCylinder(quad, 0.25, 0.25, 0.4, 16, 16);

    glColor3f(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(0, 0, 0.4);
    gluCylinder(quad, 0.35, 0.15, 0.5, 16, 16);
    glPopMatrix();

    if (lightsOn) 
    {
        GLfloat emission[] = {1.0f, 1.0f, 0.8f, 1.0f};
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
        glColor3f(1.0f, 1.0f, 0.8f);
    }
    else 
    {
        glColor3f(0.3f, 0.3f, 0.3f);
    }

    glPushMatrix();
    glTranslatef(0, 0, 0.75);
    glutSolidSphere(0.2, 16, 16);
    glPopMatrix();

    GLfloat no_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);

    gluDeleteQuadric(quad);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}
//juan
/* Fungsi untuk mengatur 4 spotlight  */
void setupSpotlights() 
{
    if (!lightsOn) 
    {
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHT3);
        glDisable(GL_LIGHT4);
        return;
    }

    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);

    GLfloat light_diffuse[] = {1.0f, 1.0f, 0.9f, 1.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    GLfloat light1_pos[] = {-5.5f, 4.8f, 5.5f, 1.0f};
    GLfloat light1_dir[] = {0.5f, -0.9f, -0.5f};
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_dir);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spotlightCutoff);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 15.0f);

    GLfloat light2_pos[] = {5.5f, 4.8f, 5.5f, 1.0f};
    GLfloat light2_dir[] = {-0.5f, -0.9f, -0.5f};
    glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_dir);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, spotlightCutoff);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 15.0f);

    GLfloat light3_pos[] = {-5.5f, 4.8f, -5.5f, 1.0f};
    GLfloat light3_dir[] = {0.5f, -0.9f, 0.5f};
    glLightfv(GL_LIGHT3, GL_POSITION, light3_pos);
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_dir);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, spotlightCutoff);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 15.0f);

    GLfloat light4_pos[] = {5.5f, 4.8f, -5.5f, 1.0f};
    GLfloat light4_dir[] = {-0.5f, -0.9f, 0.5f};
    glLightfv(GL_LIGHT4, GL_POSITION, light4_pos);
    glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, light4_dir);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular);
    glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, spotlightCutoff);
    glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 15.0f);
}

//cipa
void drawWoodBase() 
{
    glDisable(GL_BLEND);
    glPushMatrix();
    glTranslatef(0.0, -5.3, 0.0);

    glColor3f(0.4f, 0.25f, 0.15f);
    drawCube(12.0, 0.8, 12.0);

    glColor3f(0.35f, 0.2f, 0.1f);
    glTranslatef(0.0, 0.42, 0.0);
    for (int i = -5; i <= 5; i++) 
    {
        glPushMatrix();
        glTranslatef(i * 1.0f, 0.0, 0.0);
        drawCube(0.05, 0.02, 12.0);
        glPopMatrix();
    }

    glColor3f(0.8f, 0.7f, 0.3f);
    glTranslatef(0.0, 0.05, 0.0);

    glPushMatrix();
    glTranslatef(0.0, 0.0, 6.1);
    drawCube(12.2, 0.15, 0.2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, -6.1);
    drawCube(12.2, 0.15, 0.2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-6.1, 0.0, 0.0);
    drawCube(0.2, 0.15, 12.2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(6.1, 0.0, 0.0);
    drawCube(0.2, 0.15, 12.2);
    glPopMatrix();

    glPopMatrix();
}

/* Fungsi untuk menggambar frame/kerangka kaca
   Membuat border abu-abu di sekeliling panel kaca */
void drawGlassPanel(float width, float height, float depth) 
{
    glDisable(GL_BLEND);
    glColor3f(0.7f, 0.7f, 0.75f);
    float frameThick = 0.1f;

    glPushMatrix();
    glTranslatef(-width / 2, 0.0, 0.0);
    drawCube(frameThick, height + frameThick, depth + frameThick);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(width / 2, 0.0, 0.0);
    drawCube(frameThick, height + frameThick, depth + frameThick);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, height / 2, 0.0);
    drawCube(width + frameThick, frameThick, depth + frameThick);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, -height / 2, 0.0);
    drawCube(width + frameThick, frameThick, depth + frameThick);
    glPopMatrix();
}

/* Fungsi untuk menggambar keseluruhan showcase
   Merakit alas kayu, lampu spotlight, dan frame kaca */
void drawShowcase() 
{
    float showcaseHeight = 10.0f;
    float showcaseWidth = 11.0f;
    float showcaseDepth = 11.0f;
    float glassThickness = 0.15f;

    drawWoodBase();
    drawSpotlight(-5.5f, 4.8f, 5.5f);
    drawSpotlight(5.5f, 4.8f, 5.5f);
    drawSpotlight(-5.5f, 4.8f, -5.5f);
    drawSpotlight(5.5f, 4.8f, -5.5f);

    glPushMatrix();
    glTranslatef(0.0, 0.0, -showcaseDepth / 2);
    drawGlassPanel(showcaseWidth, showcaseHeight, glassThickness);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-showcaseWidth / 2, 0.0, 0.0);
    glRotatef(90, 0, 1, 0);
    drawGlassPanel(showcaseDepth, showcaseHeight, glassThickness);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(showcaseWidth / 2, 0.0, 0.0);
    glRotatef(90, 0, 1, 0);
    drawGlassPanel(showcaseDepth, showcaseHeight, glassThickness);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, showcaseHeight / 2, 0.0);
    glRotatef(90, 1, 0, 0);
    drawGlassPanel(showcaseWidth, showcaseDepth, glassThickness);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(doorSlide, 0.0, showcaseDepth / 2);
    drawGlassPanel(showcaseWidth, showcaseHeight, glassThickness);
    glPopMatrix();
}

/* Fungsi untuk menggambar semua panel kaca transparan
   Harus dipanggil terakhir agar objek di dalam tetap terlihat */
void drawAllGlass() 
{
    float showcaseHeight = 10.0f;
    float showcaseWidth = 11.0f;
    float showcaseDepth = 11.0f;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.7f, 0.85f, 0.95f, 0.2f);

    glPushMatrix();
    glTranslatef(0.0, 0.0, -showcaseDepth / 2);
    glScalef(showcaseWidth, showcaseHeight, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-showcaseWidth / 2, 0.0, 0.0);
    glScalef(0.15f, showcaseHeight, showcaseDepth);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(showcaseWidth / 2, 0.0, 0.0);
    glScalef(0.15f, showcaseHeight, showcaseDepth);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, showcaseHeight / 2, 0.0);
    glScalef(showcaseWidth, 0.15f, showcaseDepth);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(doorSlide, 0.0, showcaseDepth / 2);
    glScalef(showcaseWidth, showcaseHeight, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    glDisable(GL_BLEND);
}

/* Fungsi display - dipanggil setiap frame untuk menggambar scene
   Urutan penting: solid objects dulu, transparent objects terakhir */
void display() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(10.0, 3.0, 18.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //cam setting
	
	// angle x devil
    if (unicornMode && transition < 1.0f) 
    {
        transition += 0.02f;
    }
    else if (!unicornMode && transition > 0.0f) 
    {
        transition -= 0.02f;
    }

    if (doorOpening && doorSlide < 11.0f) 
    {
        doorSlide += 0.01f;
        if (doorSlide >= 11.0f) 
        {
            doorSlide = 11.0f;
            doorOpening = false;
        }
    }
    
    if (doorClosing && doorSlide > 0.0f) {
        doorSlide -= 0.01f;
        if (doorSlide <= 0.0f) {
            doorSlide = 0.0f;
            doorClosing = false;
        }
    }

    glRotatef(rotationAngle, 0.0, 1.0, 0.0);
    glScalef(zoom, zoom, zoom);
    
    // spotlight setting
    setupSpotlights();
    
    // Lantai (Solid)
    glPushMatrix();
    glTranslatef(0.0, -4.5, 0.0);
    glColor3f(0.5, 0.5, 0.5);
    drawCube(4.0, 0.6, 4.0);
    glPopMatrix();
    
    // Gundam dan Shiels
    drawGundam();
    // showcase
    drawShowcase();

    timeStep += 0.05f;
    for (int j = 1; j < HEIGHT_POINTS; j++) {
        float intensity = (float)j / HEIGHT_POINTS;
        for (int i = 0; i < WIDTH_POINTS; i++) {
            float wind = sin(timeStep * 0.4f + j * 0.3f) * 0.4f;
            cloak[i][j].x = cloak[i][j].baseX + (wind * 0.1f * intensity);
            cloak[i][j].z = cloak[i][j].baseZ + (wind * intensity);
        }
    }

   
    drawAllGlass();
    
    glutSwapBuffers();
    glutPostRedisplay();
}
//juan
void init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_COLOR_MATERIAL);
	GLfloat global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	
	GLfloat light0_position[] = { 0.0, 10.0, 0.0, 1.0 };
	GLfloat light0_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light0_diffuse[] = { 0.4, 0.4, 0.4, 1.0 };
	
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	
	float radiusAtas = 2.8f;
	float radiusBawah = 6.5f;
	float tinggiJubah = 8.0f;
	
	for (int j = 0; j < HEIGHT_POINTS; j++) {
	    float v = (float)j / (HEIGHT_POINTS - 1);
	    float currentRadius = radiusAtas * (0.5f - v) + radiusBawah * v;
	    for (int i = 0; i < WIDTH_POINTS; i++) {
	        float u = (float)i / (WIDTH_POINTS - 1);
	        float a = (u * 2.0f - 1.0f) * (M_PI / 2.0f);
	        cloak[i][j].baseX = currentRadius * sin(a);
	        cloak[i][j].baseY = -v * tinggiJubah;
	        cloak[i][j].baseZ = currentRadius * cos(a);
	        cloak[i][j].x = cloak[i][j].baseX;
	        cloak[i][j].y = cloak[i][j].baseY;
	        cloak[i][j].z = cloak[i][j].baseZ;
	        damageMap[i][j] = true;
	    }
	}
	
	glClearColor(0.05, 0.05, 0.1, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.33, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	}
void keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case 'x': case 'X':
	unicornMode = !unicornMode;
	break;
	case 'w': case 'W':
	zoom += 0.05f;
	break;
	case 's': case 'S':
	zoom -= 0.05f;
	break;
	case 'a': case 'A':
	rotationAngle -= 5.0f;
	break;
	case 'd': case 'D':
	rotationAngle += 5.0f;
	break;
	case 'o': case 'O':
	if (doorSlide == 0.0f) {
	doorOpening = true;
	doorClosing = false;
	} else if (doorSlide == 11.0f) {
	doorClosing = true;
	doorOpening = false;
	}
	break;
	case 'l': case 'L':
	lightsOn = !lightsOn;
	break;
	case '+': case '=':
	if (spotlightCutoff < 45.0f) {
	spotlightCutoff += 2.0f;
	}
	break;
	case '-': case '_':
	if (spotlightCutoff > 10.0f) {
	spotlightCutoff -= 2.0f;
	}
	break;
	case 'z': case 'Z':
	if (gundamScale < MAX_SCALE) {
	gundamScale += 0.05f;
	}
	break;
	case 'c': case 'C':
	if (gundamScale > MIN_SCALE) {
	gundamScale -= 0.05f;
	}
	break;
	case 27:
	exit(0);
	break;
	}
	glutPostRedisplay();
	}
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	}
int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 800);
    glutCreateWindow("Gundam Luxury Showcase");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
