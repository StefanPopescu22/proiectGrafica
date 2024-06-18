#include "glos.h"
#include "gl.h"
#include "glu.h"
#include "glaux.h"
#include <math.h>
#include <string.h>

#define PI 3.14159265

static GLfloat x = 0;
static GLfloat wheelAngle = 0;
static GLfloat truckRotationAngle = 0;
static GLsizei windowWidth = 800;
static GLsizei windowHeight = 600;

static int spin = 10;

void setLightProperties();
void setMaterialProperties();

void initializeOpenGL() {
    glClearColor(0.0, 0.7, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    setLightProperties();
    setMaterialProperties();
}

void setLightProperties() {
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void setMaterialProperties() {
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void CALLBACK moveLeft() {
    x -= 10;
}

void CALLBACK moveRight() {
    x += 10;
}

void CALLBACK rotateLeft() {
    truckRotationAngle += 10;
}

void CALLBACK rotateRight() {
    truckRotationAngle -= 10;
}

void drawCircle(GLfloat cx, GLfloat cy, GLfloat cz, GLfloat r, int num_segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(cx, cy, cz);
    for (int i = 0; i <= num_segments; i++) {
        GLfloat angle = 2 * PI * i / num_segments;
        GLfloat dx = r * cos(angle);
        GLfloat dy = r * sin(angle);
        glVertex3f(cx + dx, cy + dy, cz);
    }
    glEnd();
}

void drawWheel(GLfloat cx, GLfloat cy, GLfloat cz, GLfloat r, GLfloat width) {
    glColor3f(0.2, 0.2, 0.2);
    drawCircle(cx, cy, cz, r, 30);
    drawCircle(cx, cy, cz + width, r, 30);

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 30; i++) {
        GLfloat angle = 2 * PI * i / 30;
        GLfloat dx = r * cos(angle);
        GLfloat dy = r * sin(angle);
        glVertex3f(cx + dx, cy + dy, cz);
        glVertex3f(cx + dx, cy + dy, cz + width);
    }
    glEnd();

    GLfloat rimWidth = 0.2;
    GLfloat rimRadius = r * 0.6;
    glColor3f(0.8, 0.8, 0.8);
    drawCircle(cx, cy, cz + width / 2 - rimWidth / 2, rimRadius, 30);
    drawCircle(cx, cy, cz + width / 2 + rimWidth / 2, rimRadius, 30);

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 30; i++) {
        GLfloat angle = 2 * PI * i / 30;
        GLfloat dx = rimRadius * cos(angle);
        GLfloat dy = rimRadius * sin(angle);
        glVertex3f(cx + dx, cy + dy, cz + width / 2 - rimWidth / 2);
        glVertex3f(cx + dx, cy + dy, cz + width / 2 + rimWidth / 2);
    }
    glEnd();

    glColor3f(0.5, 0.5, 0.5);
    for (int i = 0; i < 6; i++) {
        glPushMatrix();
        glTranslatef(cx, cy, (cz + cz + width) / 2);
        glRotatef(i * 60 + wheelAngle, 0.0, 0.0, 1.0);
        glTranslatef(-cx, -cy, -((cz + cz + width) / 2));
        glBegin(GL_LINES);
        glVertex3f(cx, cy, (cz + cz + width) / 2);
        glVertex3f(cx, cy + r - 2, (cz + cz + width) / 2);
        glEnd();
        glPopMatrix();
    }
}

void CALLBACK updateWheelAngle() {
    wheelAngle -= 5;
    if (wheelAngle <= -360) {
        wheelAngle += 360;
    }
    auxIdleFunc(updateWheelAngle);
}

void drawRoad(GLsizei windowWidth) {
    GLfloat roadBottom = -10;
    GLfloat roadTop = roadBottom - 50;
    GLfloat roadDepth = -10;
    glBegin(GL_QUADS);
    glColor3f(0.3, 0.3, 0.3);
    glVertex3f(-windowWidth / 2, roadBottom, 0);
    glVertex3f(windowWidth / 2, roadBottom, 0);
    glVertex3f(windowWidth / 2, roadTop, 0);
    glVertex3f(-windowWidth / 2, roadTop, 0);

    glVertex3f(-windowWidth / 2, roadBottom, roadDepth);
    glVertex3f(windowWidth / 2, roadBottom, roadDepth);
    glVertex3f(windowWidth / 2, roadTop, roadDepth);
    glVertex3f(-windowWidth / 2, roadTop, roadDepth);

    glVertex3f(-windowWidth / 2, roadBottom, 0);
    glVertex3f(-windowWidth / 2, roadBottom, roadDepth);
    glVertex3f(-windowWidth / 2, roadTop, roadDepth);
    glVertex3f(-windowWidth / 2, roadTop, 0);

    glVertex3f(windowWidth / 2, roadBottom, 0);
    glVertex3f(windowWidth / 2, roadBottom, roadDepth);
    glVertex3f(windowWidth / 2, roadTop, roadDepth);
    glVertex3f(windowWidth / 2, roadTop, 0);

    glVertex3f(-windowWidth / 2, roadTop, 0);
    glVertex3f(-windowWidth / 2, roadTop, roadDepth);
    glVertex3f(windowWidth / 2, roadTop, roadDepth);
    glVertex3f(windowWidth / 2, roadTop, 0);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    for (GLfloat i = -windowWidth / 2; i < windowWidth / 2; i += 20) {
        glBegin(GL_QUADS);
        glVertex3f(i, roadBottom - 3.5, 0.01);
        glVertex3f(i + 10, roadBottom - 3.5, 0.01);
        glVertex3f(i + 10, roadBottom - 4, 0.01);
        glVertex3f(i, roadBottom - 4, 0.01);
        glEnd();
    }
}

void drawTruckBody() {
    GLfloat bodyBottom = -8;
    GLfloat bodyTop = -3;
    GLfloat bodyDepth = -5;
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(5, bodyBottom, 0);
    glVertex3f(20, bodyBottom, 0);
    glVertex3f(20, bodyTop, 0);
    glVertex3f(5, bodyTop, 0);

    glVertex3f(5, bodyBottom, bodyDepth);
    glVertex3f(20, bodyBottom, bodyDepth);
    glVertex3f(20, bodyTop, bodyDepth);
    glVertex3f(5, bodyTop, bodyDepth);

    glVertex3f(5, bodyBottom, 0);
    glVertex3f(5, bodyBottom, bodyDepth);
    glVertex3f(5, bodyTop, bodyDepth);
    glVertex3f(5, bodyTop, 0);

    glVertex3f(20, bodyBottom, 0);
    glVertex3f(20, bodyBottom, bodyDepth);
    glVertex3f(20, bodyTop, bodyDepth);
    glVertex3f(20, bodyTop, 0);

    glVertex3f(5, bodyTop, 0);
    glVertex3f(5, bodyTop, bodyDepth);
    glVertex3f(20, bodyTop, bodyDepth);
    glVertex3f(20, bodyTop, 0);

    glVertex3f(5, bodyBottom, 0);
    glVertex3f(5, bodyBottom, bodyDepth);
    glVertex3f(20, bodyBottom, bodyDepth);
    glVertex3f(20, bodyBottom, 0);
    glEnd();
}

void drawTruckCabin() {
    GLfloat cabinBottom = -3;
    GLfloat cabinTop = 2;
    GLfloat cabinDepth = -5;
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(15, cabinBottom, 0);
    glVertex3f(20, cabinBottom, 0);
    glVertex3f(20, cabinTop, 0);
    glVertex3f(15, cabinTop, 0);

    glVertex3f(15, cabinBottom, cabinDepth);
    glVertex3f(20, cabinBottom, cabinDepth);
    glVertex3f(20, cabinTop, cabinDepth);
    glVertex3f(15, cabinTop, cabinDepth);

    glVertex3f(15, cabinBottom, 0);
    glVertex3f(15, cabinBottom, cabinDepth);
    glVertex3f(15, cabinTop, cabinDepth);
    glVertex3f(15, cabinTop, 0);

    glVertex3f(20, cabinBottom, 0);
    glVertex3f(20, cabinBottom, cabinDepth);
    glVertex3f(20, cabinTop, cabinDepth);
    glVertex3f(20, cabinTop, 0);

    glVertex3f(15, cabinTop, 0);
    glVertex3f(15, cabinTop, cabinDepth);
    glVertex3f(20, cabinTop, cabinDepth);
    glVertex3f(20, cabinTop, 0);

    glVertex3f(15, cabinBottom, 0);
    glVertex3f(15, cabinBottom, cabinDepth);
    glVertex3f(20, cabinBottom, cabinDepth);
    glVertex3f(20, cabinBottom, 0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.5, 0.8, 1.0);
    glVertex3f(16, cabinBottom + 1, 0.01);
    glVertex3f(19, cabinBottom + 1, 0.01);
    glVertex3f(19, cabinTop - 1, 0.01);
    glVertex3f(16, cabinTop - 1, 0.01);

    glVertex3f(16, cabinBottom + 1, cabinDepth + 0.01);
    glVertex3f(19, cabinBottom + 1, cabinDepth + 0.01);
    glVertex3f(19, cabinTop - 1, cabinDepth + 0.01);
    glVertex3f(16, cabinTop - 1, cabinDepth + 0.01);
    glEnd();
}

void drawTruckDetails() {
    glBegin(GL_QUADS);
    glColor3f(0.5, 0.5, 0.5);
    glVertex3f(19, 2, -0.5);
    glVertex3f(19.5, 2, -0.5);
    glVertex3f(19.5, 4, -0.5);
    glVertex3f(19, 4, -0.5);

    glVertex3f(19, 2, -1.5);
    glVertex3f(19.5, 2, -1.5);
    glVertex3f(19.5, 4, -1.5);
    glVertex3f(19, 4, -1.5);

    glVertex3f(19, 4, -0.5);
    glVertex3f(19.5, 4, -0.5);
    glVertex3f(19.5, 4, -1.5);
    glVertex3f(19, 4, -1.5);

    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(20.1, -7.5, -0.5);
    glVertex3f(20.1, -7.5, -4.5);
    glVertex3f(20.1, -6.5, -4.5);
    glVertex3f(20.1, -6.5, -0.5);

    glEnd();
}

void drawTruck() {
    drawTruckBody();
    drawTruckCabin();
    drawTruckDetails();
    drawWheel(7, -10, 0, 1, 0.5);
    drawWheel(18, -10, 0, 1, 0.5);
    drawWheel(7, -10, -5, 1, 0.5);
    drawWheel(18, -10, -5, 1, 0.5);
}

void drawSun() {
    glColor3f(1.0, 1.0, 0.0);
    drawCircle(13, 13, -5, 2, 30);
}

void calculateShadowMatrix(GLfloat shadowMatrix[4][4], GLfloat lightPos[4], GLfloat groundPlane[4]) {
    GLfloat dot = groundPlane[0] * lightPos[0] +
        groundPlane[1] * lightPos[1] +
        groundPlane[2] * lightPos[2] +
        groundPlane[3] * lightPos[3];

    shadowMatrix[0][0] = dot - lightPos[0] * groundPlane[0];
    shadowMatrix[1][0] = 0.0f - lightPos[0] * groundPlane[1];
    shadowMatrix[2][0] = 0.0f - lightPos[0] * groundPlane[2];
    shadowMatrix[3][0] = 0.0f - lightPos[0] * groundPlane[3];

    shadowMatrix[0][1] = 0.0f - lightPos[1] * groundPlane[0];
    shadowMatrix[1][1] = dot - lightPos[1] * groundPlane[1];
    shadowMatrix[2][1] = 0.0f - lightPos[1] * groundPlane[2];
    shadowMatrix[3][1] = 0.0f - lightPos[1] * groundPlane[3];

    shadowMatrix[0][2] = 0.0f - lightPos[2] * groundPlane[0];
    shadowMatrix[1][2] = 0.0f - lightPos[2] * groundPlane[1];
    shadowMatrix[2][2] = dot - lightPos[2] * groundPlane[2];
    shadowMatrix[3][2] = 0.0f - lightPos[2] * groundPlane[3];

    shadowMatrix[0][3] = 0.0f - lightPos[3] * groundPlane[0];
    shadowMatrix[1][3] = 0.0f - lightPos[3] * groundPlane[1];
    shadowMatrix[2][3] = 0.0f - lightPos[3] * groundPlane[2];
    shadowMatrix[3][3] = dot - lightPos[3] * groundPlane[3];
}

void drawShadow(GLfloat lightPos[4]) {
    GLfloat groundPlane[4] = { 0.0f, 1.0f, 0.0f, 10.0f };
    GLfloat shadowMatrix[4][4];
    calculateShadowMatrix(shadowMatrix, lightPos, groundPlane);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0, 0.0, 0.0); // Black color for the shadow

    glPushMatrix();
    glMultMatrixf((GLfloat*)shadowMatrix);
    glTranslatef(x / 10.0, 0, 0.0);
    glRotatef(truckRotationAngle, 0.0, 1.0, 0.0);
    drawTruck();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

// Bitmap data for the characters (8x8), flipped vertically
GLubyte P_Bitmap[] = { 0x80, 0x80, 0x80, 0x80, 0xFE, 0x82, 0x82, 0xFE };
GLubyte o_Bitmap[] = { 0x00, 0x7C, 0x82, 0x82, 0x82, 0x82, 0x7C, 0x00 };
GLubyte p_Bitmap[] = { 0x80, 0x80, 0x80, 0xFC, 0x82, 0x82, 0xFC, 0x00 };
GLubyte e_Bitmap[] = { 0x00, 0x7C, 0x82, 0x80, 0xFE, 0x82, 0x7C, 0x00 };
GLubyte s_Bitmap[] = { 0x00, 0xFC, 0x02, 0x02, 0x7C, 0x80, 0x7E, 0x00 };
GLubyte c_Bitmap[] = { 0x00, 0x7C, 0x82, 0x80, 0x80, 0x82, 0x7C, 0x00 };
GLubyte u_Bitmap[] = { 0x00, 0x7E, 0x82, 0x82, 0x82, 0x82, 0x82, 0x00 };
GLubyte S_Bitmap[] = { 0x00, 0x7C, 0x82, 0x02, 0x7C, 0x80, 0x82, 0x7C };
GLubyte t_Bitmap[] = { 0x00, 0x1C, 0x22, 0x20, 0x20, 0x20, 0xFE, 0x00 };
GLubyte f_Bitmap[] = { 0x00, 0x40, 0x40, 0x40, 0xFC, 0x40, 0x3E, 0x00 };
GLubyte a_Bitmap[] = { 0x00, 0x7E, 0x82, 0x82, 0x7E, 0x02, 0x7C, 0x00 };
GLubyte n_Bitmap[] = { 0x00, 0x82, 0x82, 0x82, 0x82, 0xC2, 0xBC, 0x00 };
GLubyte T_Bitmap[] = { 0x00, 0x28, 0x28, 0x28, 0x28, 0x28, 0xFE, 0x00 };
GLubyte I_Bitmap[] = { 0x00, 0x38, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38 };
GLubyte space_Bitmap[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void renderCharacter(char c) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    switch (c) {
        case 'P': glBitmap(8, 8, 0.0, 0.0, 10.0, 0.0, P_Bitmap); break;
        case 'o': glBitmap(8, 8, 0.0, 0.0, 10.0, 0.0, o_Bitmap); break;
        case 'p': glBitmap(8, 8, 0.0, 0.0, 10.0, 0.0, p_Bitmap); break;
        case 'e': glBitmap(8, 8, 0.0, 0.0, 10.0, 0.0, e_Bitmap); break;
        case 's': glBitmap(8, 8, 0.0, 0.0, 10.0, 0.0, s_Bitmap); break;
        case 'c': glBitmap(8, 8, 0.0, 0.0, 10.0, 0.0, c_Bitmap); break;
        case 'u': glBitmap(8, 8, 0.0, 0.0, 10.0, 0.0, u_Bitmap); break;
        case 'S': glBitmap(8, 8, 0.0, 0.0, 10.0, 0.0, S_Bitmap); break;
        case 't': glBitmap(8, 8, 0.0, 0.0, 10.0, 0.0, t_Bitmap); break;
        case 'f': glBitmap(8, 8, 0.0, 0.0, 10.0, 0.0, f_Bitmap); break;
        case 'a': glBitmap(8, 8, 0.0, 0.0, 10.0, 0.0, a_Bitmap); break;
        case 'n': glBitmap(8, 8, 0.0, 0.0, 10.0, 0.0, n_Bitmap); break;
        case 'T': glBitmap(8, 8, 0.0, 0.0, 10.0, 0.0, T_Bitmap); break;
        case 'I': glBitmap(8, 8, 0.0, 0.0, 10.0, 0.0, I_Bitmap); break;
        case ' ': glBitmap(8, 8, 0.0, 0.0, 10.0, 0.0, space_Bitmap); break;
        default: break;
    }
}

void renderText(const char* text, float x, float y, float z) {
    glColor3f(1.0, 0.0, 0.0); // Red color for the text
    glRasterPos3f(x, y, z);
    while (*text) {
        renderCharacter(*text);
        text++;
    }
}

void drawTruckWithText() {
    drawTruck();
    renderText("Popescu Stefan TI", -20.0f, -15.0f, 0.0f);
}

void CALLBACK display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 20.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    drawSun();

    GLfloat lightPos[] = { 13.0, 13.0, -5.0, 1.0 };
    glPushMatrix();
    glRotated((GLdouble)spin, 1.0, 0.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glPopMatrix();

    drawRoad(500);

    glPushMatrix();
    glTranslatef(x / 10.0, 0, 0.0);
    glRotatef(truckRotationAngle, 0.0, 1.0, 0.0);
    drawTruckWithText();
    glPopMatrix();

    drawShadow(lightPos);

    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h) {
    if (!h) return;
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    auxInitDisplayMode(AUX_SINGLE | AUX_RGB | AUX_DEPTH);
    auxInitPosition(0, 0, 800, 600);
    auxInitWindow("Un camion");
    initializeOpenGL();
    auxKeyFunc(AUX_LEFT, moveLeft);
    auxKeyFunc(AUX_RIGHT, moveRight);
    auxKeyFunc('w', rotateLeft);
    auxKeyFunc('s', rotateRight);
    auxIdleFunc(updateWheelAngle);
    auxReshapeFunc(myReshape);
    auxMainLoop(display);
    return 0;
}
