#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "BarnesHut.h"


// Initialise
Cell* new_cell()
{
  Cell* c = malloc(sizeof(Cell));
  c->size = 0.0;
  c->offset[0] = 0.0; c->offset[1] = 0.0;
  c->particle.mass = 0.0; 
  c->particle.x[0] = 0.0; c->particle.x[1] = 0.0; 

  c->quad[0] = NULL; 
  c->quad[1] = NULL; 
  c->quad[2] = NULL; 
  c->quad[3] = NULL; 

  return c; 
}

void get_centre_of_mass(Cell* c)
{
  int i,k;
  for (i=0;i<4;i++) 
    if (c->quad[i]!=NULL)   
    {
      get_centre_of_mass(c->quad[i]);
      c->particle.mass += c->quad[i]->particle.mass; 
    }

  for (i=0;i<4;i++) 
    if (c->quad[i]!=NULL)   
      for (k=0;k<2;k++)
        c->particle.x[k] += 
          (c->quad[i]->particle.mass * c->quad[i]->particle.x[k])/
           c->particle.mass; 

  // printf("COM: %lf %lf\n", 
    // c->particle.x[0], c->particle.x[1]);
}


void into_quadrants(int n, Particle* p, Cell *c)
{
  Vec2 mid,diff; 
  double size2 = c->size * 0.5; 
  int quad_n[4] = {0,0,0,0};
  int i,j;
  Particle* pp[4];

  static int quad_off[4][2] = {{0,0},{1,0},{0,1},{1,1}};

  mid[0] = c->offset[0] + size2;
  mid[1] = c->offset[1] + size2;

  for (i=0;i<n;i++)
  {
    diff[0] = p[i].x[0] - mid[0];
    diff[1] = p[i].x[1] - mid[1];
    if (diff[0]<0.0 && diff[1] < 0.0) 
    {
      quad_n[0]++;
    }
    if (diff[0]>0.0 && diff[1] < 0.0) 
    {
      quad_n[1]++;
    }
    if (diff[0]<0.0 && diff[1] > 0.0) 
    {
      quad_n[2]++;
    }
    if (diff[0]>0.0 && diff[1] > 0.0) 
    {
      quad_n[3]++;
    }
  }
  // printf("in each quadrant: %d %d %d %d\n",
      //quad_n[0], quad_n[1], quad_n[2], quad_n[3]);

  for (i=0;i<4;i++)
  {
    if (quad_n[i] > 0) pp[i] = malloc(sizeof(Particle) * quad_n[i]); 
    quad_n[i]=0;
  }


  for (i=0;i<n;i++)
  {
    diff[0] = p[i].x[0] - mid[0];
    diff[1] = p[i].x[1] - mid[1];
    if (diff[0]<0.0 && diff[1] < 0.0) 
    {
      pp[0][quad_n[0]] = p[i];
      quad_n[0]++;
    }
    if (diff[0]>0.0 && diff[1] < 0.0) 
    {
      pp[1][quad_n[1]] = p[i];
      quad_n[1]++;
    }
    if (diff[0]<0.0 && diff[1] > 0.0) 
    {
      pp[2][quad_n[2]] = p[i];
      quad_n[2]++;
    }
    if (diff[0]>0.0 && diff[1] > 0.0) 
    {
      pp[3][quad_n[3]] = p[i];
      quad_n[3]++;
    }
  }

#if 0 
// --
  for (i=0;i<4;i++)
  {
    if (quad_n[i]>1) 
    {
      printf("quadrant %d\n",i);
      for (j=0;j<quad_n[i];j++)
        printf("%lf %lf\n",pp[i][j].x[0],pp[i][j].x[1]);
    }
  }
// --
#endif 

  for (i=0;i<4;i++)
  {
    if (quad_n[i] == 0) // empty cell
    {
      c->quad[i] = NULL;
    }
    else if (quad_n[i] == 1) // cell with one planet
    {
      c->quad[i] = new_cell();
      c->quad[i]->size = 0.0;
      c->quad[i]->particle = pp[i][0];
    }
    else 
    {
      c->quad[i] = new_cell();
      c->quad[i]->size = size2;
      c->quad[i]->offset[0]=c->offset[0] + size2 * quad_off[i][0]; 
      c->quad[i]->offset[1]=c->offset[1] + size2 * quad_off[i][1]; 
      into_quadrants(quad_n[i], pp[i], c->quad[i]);
    }
  }

}

void free_tree(Cell* c)
{
  int i,k=0;
  for (i=0;i<4;i++)
    if (c->quad[i] != NULL) 
    {
      free_tree(c->quad[i]); 
      free(c->quad[i]); 
      c->quad[i]=NULL;
    }
}

// Test - print 
void print(Cell* head)
{
  int i,k=0;
  for (i=0;i<4;i++) 
  {
    if (head->quad[i]!=NULL)
    {
      print(head->quad[i]); 
    }
    else k++; 
  }
#if 0
  if (k==4 && head->particle.mass != 0) 
    printf("%lf %lf\n",head->particle.x[0],head->particle.x[1]);
  else if (k>0)
    printf("COM %lf %lf\n",head->particle.x[0],head->particle.x[1]);
#endif 
 
}

void add_force(double theta, int k, Vec2 x_k, Cell* c, Vec2 force)
{
  int j;
  Vec2 d = {c->particle.x[0]-x_k[0], c->particle.x[1]-x_k[1]}; 
  double r; 

  if (c->size > 0.0)
    r = sqrt(d[0]*d[0]+d[1]*d[1]) / c->size;
  else 
    r = 0.0;

// printf("c->size=%lf, r=%lf\n",c->size,r);

  if (r<theta) 
  {
    for (j=0;j<4;j++)
      if (c->quad[j] != NULL)
      {
        add_force(theta,k,x_k,c->quad[j],force); 
      }
  }
  else 
  {
    r = sqrt(d[0]*d[0]+d[1]*d[1]); 
    
    force[0] += c->particle.mass * d[0] / (r*r*r); 
    force[1] += c->particle.mass * d[1] / (r*r*r); 
  }

  

  

}
