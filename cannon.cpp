// cannon.cpp : Defines the entry point for the console application.
//
// Pre-Workshop
//
// Goal: Get to know the code you will start with for Workshop 1
//
// The workshops are timed problem sessions where your solutions
// must be turned in at the end of the workshop (like an exam).
//
// You are encouraged to received advice and tips from the teaching assistant
// and other students, but the source code must be written by you
//
// The workshops are typically done with 2 students per computer.
//
// For workshops, note that each and every student must make their
// own submission on the LML Course Manager.  If John and Mary do the
// workshop together then John must make a submission and also Mary
// must make a submission.  Late submissions are subject to the late penalty.
// If Mary assumes that John's submission includes hers, and does not
// make her own submission, her grade will be zero.
//
// See class notes for explanation of functions
//

//#include "stdafx.h"
#include <stdlib.h>
#include "GL/glut.h"
#define _USE_MATH_DEFINES
#define MAXPARTICLES 20
#include <math.h>
#include <time.h>
#include <iostream>

float posx_init = 0.0, posy_init = 25.0, posz_init = 50.0, orientation_init = 15.0;
float g_posX = posx_init, g_posY = posy_init, g_posZ = posy_init;
float g_orientation = orientation_init; // y axis
float posy = 0, randR, randG, randB;
bool gravity = true;
float firehoogte = 20.0f;
float gravity_force = 15.0f;
float scale_over_time = 0.1f;
bool descending;

struct pinfo
{
    float width;
    float x, y, z;
    float v_x, v_y, v_z;
    float r, g, b;
} particles[1000];

void fireCannon()
{
    unsigned int i;
    descending = false;
    for (i = 0; i < MAXPARTICLES; i = i + 1)
    {
        particles[i].width = 3.0 * (rand() / (float)RAND_MAX) + 1.0;
        particles[i].x = 0.0;
        particles[i].y = 0.0;
        particles[i].z = 0.0;
        particles[i].v_x = 10.0 * (rand() / (float)RAND_MAX) - 5.0f;
        particles[i].v_y = 30.0f;
        particles[i].v_z = 10.0 * (rand() / (float)RAND_MAX) - 5.0f;
        particles[i].r = 1.0 * (rand() / (float)RAND_MAX);
        particles[i].g = 1.0 * (rand() / (float)RAND_MAX);
        particles[i].b = 1.0 * (rand() / (float)RAND_MAX);
    }
    glutGet(GLUT_ELAPSED_TIME);
}

void drawOneParticle()
{

    glBegin(GL_TRIANGLE_STRIP);
    // triangle 1
    glVertex3f(-0.5, 0.0, 0.5); // A
    glVertex3f(0.0, 0.0, -0.5); // B
    glVertex3f(0.0, 1.0, 0.0);  // top
    // triangle 2
    glVertex3f(0.5, 0.0, 0.5); // C
    // triangle 3
    glVertex3f(-0.5, 0.0, 0.5); // A again
    // triangle 4 (bottom)
    glVertex3f(0.0, 0.0, -0.5); // B again
    glEnd();
}

void drawParticles()
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
        posy = 0;
        fireCannon();
        break;
    case 'g':
        gravity = !gravity;
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
    glVertex3f(-40.0, 0.0, -40.0);
    glVertex3f(-40.0, 0.0, 40.0);
    glVertex3f(40.0, 0.0, 40.0);
    glVertex3f(40.0, 0.0, -40.0);
    glVertex3f(-40.0, 0.0, -40.0);
    glEnd();

    drawParticles();
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

    for (i = 0; i < MAXPARTICLES; i = i + 1)
    {
        //gravity simulation
        particles[i].v_y = gravity ? particles[i].v_y - gravity_force * time : particles[i].v_y;

        //stop when hit ground
        if (particles[i].y < 0)
        {
            particles[i].v_x = 0;
            particles[i].v_y = 0;
            particles[i].v_z = 0;
        }
        //check time for exploade
        if (particles[i].y >= firehoogte)
        {
            descending = true;
        }

        if (descending)
        {
            particles[i].x = particles[i].x + particles[i].v_x * time;
            particles[i].z = particles[i].z + particles[i].v_z * time;
            if (particles[i].width > 0.0f)
            {
                particles[i].width -= scale_over_time;
            }
        }

        particles[i].y = particles[i].y + particles[i].v_y * time;
    }
    glutPostRedisplay();
    glutTimerFunc(50, &timer, 0);
}

int main(int argc, char *argv[])
{
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
