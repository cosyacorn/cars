#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "BarnesHut.h"

int main()
{
  int n=100000;
  Particle p[n];
  Vec2* force = malloc(sizeof(Vec2)*n); 
  int i;
  double theta=1000.0;

  srand48(12456);

  for (i=0;i<n;i++)
  {
    p[i].mass = 0.1;
    p[i].x[0] = drand48(); p[i].x[1] = drand48();
    force[i][0] = 0.0; force[i][1] = 0.0;
  }

  eval_force(n,p,force);


}
