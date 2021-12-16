#define MAXPARTICLES 20

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
  Firework(float x, float y, float z, float hoogte, float speed)
  {
    posx = x;
    posy = y;
    posz = z;
    firehoogte = hoogte;
    gravity = true;
    launchspeed = speed;
  }
  ~Firework(){};

  void fire()
  {
    descending = false;
    for (int i = 0; i < MAXPARTICLES; i = i + 1)
    {
      particles[i].width = 0.5f;
      particles[i].x = posx;
      particles[i].y = posy;
      particles[i].z = posz;
      particles[i].v_x = 10.0 * (rand() / (float)RAND_MAX) - 5.0f;
      particles[i].v_y = launchspeed;
      particles[i].v_z = 10.0 * (rand() / (float)RAND_MAX) - 5.0f;
      particles[i].r = 1.0 * (rand() / (float)RAND_MAX);
      particles[i].g = 1.0 * (rand() / (float)RAND_MAX);
      particles[i].b = 1.0 * (rand() / (float)RAND_MAX);
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
        particles[i].x = particles[i].x + particles[i].v_x * time;
        particles[i].z = particles[i].z + particles[i].v_z * time;
        if (particles[i].width > 0.0f)
        {
          particles[i].width -= scale_over_time;
        }
      }

      particles[i].y = particles[i].y + particles[i].v_y * time;
    }
  }

  void toggleGravity()
  {
    gravity = !gravity;
  }

  pinfo *getParticles()
  {
    return particles;
  }

private:
  pinfo particles[1000];
  float gravity_force = 15.0f;
  float scale_over_time = 0.01f;
  bool descending;
  bool gravity;
  float firehoogte;
  float posx, posy, posz;
  float launchspeed;
};