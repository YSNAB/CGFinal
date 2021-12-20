#include <stdlib.h>
#include "GL/glut.h"
#define _USE_MATH_DEFINES
#define MAXPARTICLES 100
#define PLANE 40.0
#include <math.h>
#include <time.h>
#include <iostream>

#include "SDL2/SDL.h"
#include "audio.h"

#include "firework.h"

float posx_init = 0.0, posy_init = 25.0, posz_init = 50.0, orientation_init = 15.0;
float g_posX = posx_init, g_posY = posy_init, g_posZ = posy_init;
float g_orientation = orientation_init; // y axis
float randR, randG, randB;

Firework fw;

void fireCannon()
{
    float zpos = 0.0f;
    float xpos = 0.0f;
    float hoogte = 40.0f;
    float speed = 50.0f;
    float type = rand() % 3;
    float r = rand() / (float)RAND_MAX;
    float g = rand() / (float)RAND_MAX;
    float b = rand() / (float)RAND_MAX;

    fw = Firework(xpos, 0, zpos, hoogte, speed, r, g, b, type);

    fw.fire();
}

void drawOneParticle()
{
    glBegin(GL_QUADS); // Begin drawing the color cube with 6 quads

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

    glDisable(GL_LIGHT1);
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
        fw.toggleGravity();
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

    if (fw.getDescending())
    {
        if (!fw.getSounded())
        {
            std::cout << "Sound" << std::endl;
            playSound("explosion.wav", SDL_MIX_MAXVOLUME / 2);
            fw.setSounded(true);
        }
        GLfloat mat_specular[] = {
            fw.getParticles()[0].width * 2,
            fw.getParticles()[0].width * 2,
            fw.getParticles()[0].width * 2, 1.0};

        GLfloat mat_ambient[] = {fw.getParticles()[0].r * fw.getParticles()[0].width * 10,
                                 fw.getParticles()[0].g * fw.getParticles()[0].width * 10,
                                 fw.getParticles()[0].b * fw.getParticles()[0].width * 10, 1.0};
        GLfloat diffuseLight[] = {0.8, 0.8, 0.8, 1.0};
        GLfloat mat_shininess[] = {50.0};

        GLfloat spot_direction[] = {0.0, -1.0, 0.0};

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glShadeModel(GL_SMOOTH);

        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 5000);

        GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0};
        fw.getGemX(light_position);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    }

    // ground plane
    glBegin(GL_QUADS);
    glNormal3d(0, 1, 0);
    glColor3b(0, 0, 0);
    glVertex3f(-PLANE, 0.0, -PLANE);
    glVertex3f(-PLANE, 0.0, PLANE);
    glVertex3f(PLANE, 0.0, PLANE);
    glVertex3f(PLANE, 0.0, -PLANE);
    glVertex3f(-PLANE, 0.0, -PLANE);
    glEnd();

    glDisable(GL_LIGHTING);

    drawParticles(fw.getParticles());

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

    fw.update(time);

    glutPostRedisplay();
    glutTimerFunc(50, &timer, 0);
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        return 1;
    }
    initAudio();

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
