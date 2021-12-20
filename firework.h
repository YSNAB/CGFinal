#define MAXPARTICLES 100

struct pinfo
{
  float width;
  float x, y, z;
  float v_x, v_y, v_z;
  float r, g, b;
};

class Firework
{
public:
  Firework(){};
  Firework(float x, float y, float z, float hoogte, float speed, float r, float g, float b, float typeex)
  {
    posx = x;
    posy = y;
    posz = z;
    firehoogte = hoogte;
    gravity = true;
    launchspeed = speed;
    color_r = r;
    color_g = g;
    color_b = b;
    typeExplosion = typeex;
    sounded = false;
  }
  ~Firework(){};

  void fire()
  {
    descending = false;
    for (int i = 0; i < MAXPARTICLES; i = i + 1)
    {
      particles[i].width = 1.0f;
      particles[i].x = posx;
      particles[i].y = posy;
      particles[i].z = posz;
      particles[i].v_x = 10.0 * (rand() / (float)RAND_MAX) - 5.0f;
      particles[i].v_y = launchspeed;
      particles[i].v_z = 10.0 * (rand() / (float)RAND_MAX) - 5.0f;
      particles[i].r = color_r;
      particles[i].g = color_g;
      particles[i].b = color_b;
    }
  }

  void update(float time)
  {

    for (int i = 0; i < MAXPARTICLES; i = i + 1)
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
        if (typeExplosion == 0)
        {
          particles[i].x = particles[i].x + particles[i].v_x * time;
          particles[i].z = particles[i].z + particles[i].v_z * time;
        }
        else if (typeExplosion == 1)
        {
          float x = cos(i * 2 * M_PI / MAXPARTICLES);
          float z = sin(i * 2 * M_PI / MAXPARTICLES);
          particles[i].x += x;
          particles[i].z += z;
        }
        else if (typeExplosion == 2)
        {
          float circleAngle = i * 2 * M_PI / sqrt(MAXPARTICLES);
          for (int j = 0; j < sqrt(MAXPARTICLES); j++)
          {
            float x = cos(j * 2 * M_PI / sqrt(MAXPARTICLES)) * cos(circleAngle);
            float y = sin(j * 2 * M_PI / sqrt(MAXPARTICLES)) * cos(circleAngle);
            float z = sin(circleAngle);
            int index = j + i * (int)sqrt(MAXPARTICLES);
            particles[index].x += x;
            particles[index].y += y;
            particles[index].z += z;
          }
        }
        else if (typeExplosion == 3)
        {
          float x = cos(i * 2 * M_PI / MAXPARTICLES);
          float z = sin(i * 2 * M_PI / MAXPARTICLES);
          float y = 0;
          if (i % 3 == 0)
          {
            y = x;
          }
          else if (i % 3 == 1)
          {
            y = -x;
          }
          else
          {
            y = -z;
          }
          particles[i].x += x;
          particles[i].y += y;
          particles[i].z += z;
        }
        if (particles[i].width > 0.0f)
        {
          particles[i].width -= scale_over_time;
        }
        else if (particles[i].width < 0.0f)
        {
          particles[i].width = 0;
        }
      }

      particles[i].y = particles[i].y + particles[i].v_y * time;
    }
  }

  void getGemX(GLfloat *lpos)
  {

    lpos[0] = 0.0;
    lpos[1] = 0.0;
    lpos[2] = 0.0;
    for (int i = 0; i < MAXPARTICLES; i++)
    {
      lpos[0] += particles[i].x;
      lpos[1] += particles[i].y;
      lpos[2] += particles[i].z;
    }
    lpos[0] /= MAXPARTICLES;
    lpos[1] /= MAXPARTICLES;
    lpos[2] /= MAXPARTICLES;
  }

  void toggleGravity()
  {
    gravity = !gravity;
  }

  pinfo *getParticles()
  {
    return particles;
  }

  float getX()
  {
    return posx;
  }

  float getY()
  {
    return posy;
  }

  float getZ()
  {
    return posz;
  }

  bool getDescending()
  {
    return descending;
  }

  bool getSounded()
  {
    return sounded;
  }

  void setSounded(bool to)
  {
    sounded = to;
  }

private:
  int typeExplosion;
  pinfo particles[1000];
  float gravity_force = 15.0f;
  float scale_over_time = 0.02f;
  bool descending;
  bool gravity;
  float firehoogte;
  float posx, posy, posz;
  float launchspeed;
  float color_r;
  float color_g;
  float color_b;
  bool sounded;
};