#include <stdlib.h>
#include "GL/glut.h"
#define _USE_MATH_DEFINES
#define MAXPARTICLES 20
#define NFIREWORKS 1
#define PLANE 40.0
#include <math.h>
#include <time.h>
#include <iostream>

#include "firework.h"

float posx_init = 0.0, posy_init = 25.0, posz_init = 50.0, orientation_init = 15.0;
float g_posX = posx_init, g_posY = posy_init, g_posZ = posy_init;
float g_orientation = orientation_init; // y axis
float randR, randG, randB;

Firework elements[NFIREWORKS];

void fireCannon()
{
    for (int i = 0; i < NFIREWORKS; i++)
    {
        elements[i].fire();
    }
}

void drawOneParticle()
{

    glBegin(GL_QUADS); // Begin drawing the color cube with 6 quads
                       // Top face (y = 1.0f)
                       // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Bottom face (y = -1.0f)
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Front face  (z = 1.0f)
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f)
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face (x = 1.0f)
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd(); // End of drawing color-cube
}

void drawParticles(pinfo *particles)
{
    unsigned int i;
    for (i = 0; i < MAXPARTICLES; i = i + 1)
    {

        glPushMatrix();
        glTranslatef(particles[i].x, particles[i].y, particles[i].z);
        glScalef(particles[i].width, particles[i].width, particles[i].width);
        glColor3f(particles[i].r, particles[i].g, particles[i].b);
        drawOneParticle();
        glPopMatrix();
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a': // up
        g_posY = g_posY + 1.0;
        break;
    case 'z': // down
        g_posY = g_posY - 1.0;
        break;
    case 'j': // left
        g_orientation = g_orientation - 15.0;
        break;
    case 'l': // right
        g_orientation = g_orientation + 15.0;
        break;
    case 'i': // forwards
        g_posX = g_posX + sin(g_orientation / 180.0 * M_PI);
        g_posZ = g_posZ - cos(g_orientation / 180.0 * M_PI);
        break;
    case 'k':
        g_posX = g_posX - sin(g_orientation / 180.0 * M_PI);
        g_posZ = g_posZ + cos(g_orientation / 180.0 * M_PI);
        break;
    case 'f': // fire
        fireCannon();
        break;
    case 'g':
        for (int i = 0; i < NFIREWORKS; i++)
        {
            elements[i].toggleGravity();
        }
        break;

    case 'q': // exit
        exit(0);
        break;
    case 'h':
        g_posX = posx_init, g_posY = posy_init, g_posZ = posz_init;
        g_orientation = orientation_init; // y axis
        break;
    }
    glutPostRedisplay();
}

void update()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(g_orientation, 0.0, 1.0, 0.0); // rotate in y axis
    glTranslatef(-g_posX, -g_posY, -g_posZ);

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    // cannon base
    glBegin(GL_QUADS);
    glVertex3f(-5.0, 0.0, -5.0);
    glVertex3f(-5.0, 0.0, 5.0);
    glVertex3f(5.0, 0.0, 5.0);
    glVertex3f(5.0, 0.0, -5.0);
    glEnd();
    // ground plane
    glBegin(GL_LINE_STRIP);
    glVertex3f(-PLANE, 0.0, -PLANE);
    glVertex3f(-PLANE, 0.0, PLANE);
    glVertex3f(PLANE, 0.0, PLANE);
    glVertex3f(PLANE, 0.0, -PLANE);
    glVertex3f(-PLANE, 0.0, -PLANE);
    glEnd();

    for (int i = 0; i < NFIREWORKS; i++)
    {
        drawParticles(elements[i].getParticles());
    }

    glutSwapBuffers();
}

void timer(int value)
{
    unsigned int i;
    static int lastTime;
    int thisTime;
    float time;

    thisTime = glutGet(GLUT_ELAPSED_TIME);
    time = (thisTime - lastTime) / 500.0;
    lastTime = thisTime;

    for (int i = 0; i < NFIREWORKS; i++)
    {
        elements[i].update(time);
    }

    glutPostRedisplay();
    glutTimerFunc(50, &timer, 0);
}

int main(int argc, char *argv[])
{
    float xpos[3] = {-20.0f, 10.0f, 15.0f};
    float zpos[3] = {-10.0f, 14.0f, 7.0f};
    float hoogte[3] = {20.0f, 20.0f, 40.0f};
    float speeds[3] = {30.0f, 30.0f, 60.0f};
    for (int i = 0; i < NFIREWORKS; i++)
    {
        elements[i] = Firework(xpos[i], 0, zpos[i], hoogte[i], speeds[i]);
    }
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutCreateWindow("Particle Cannon");
    glMatrixMode(GL_PROJECTION);
    gluPerspective(120.0, 1.0, 1.0, 1000.0);
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(&update);
    glutKeyboardFunc(&keyboard);
    glutTimerFunc(50, &timer, 0);
    glutMainLoop();
    return 0;
}
