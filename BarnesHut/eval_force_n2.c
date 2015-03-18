#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "BarnesHut.h"


void add_force_one(int i, int n, Particle p[n], Vec2 force)
{
  Vec2 d; 
  double r; 
  int j;

  for (j=0;j<n;j++)
    if (i!=j)
    {
      d[0]=p[i].x[0]-p[j].x[0];
      d[1]=p[i].x[1]-p[j].x[1];

      r = sqrt(d[0]*d[0]+d[1]*d[1]); 
   
      force[0] += p[j].mass * d[0] / (r*r*r); 
      force[1] += p[j].mass * d[1] / (r*r*r); 
    }
}

void eval_force(int n, Particle p[n], Vec2* force)
{
  int id,nt,i_hi,i_lo; // OpemMP
  int i;

#pragma omp parallel shared(p,n,force) private(id,nt,i,i_hi,i_lo)
{
  id=omp_get_thread_num();
  nt=omp_get_num_threads(); 

  i_lo=(id*n)/nt;
  i_hi=((id+1)*n)/nt;
  // printf("%d %d [%d %d]\n",id,nt,i_lo,i_hi);
  if (i_hi>n) i_hi=n;
  
  for (i=i_lo;i<i_hi;i++) 
    add_force_one(i,n,p,force[i]); 
}
  
  // for (i=0;i<n;i++) 
    // printf("force[%d]=%lf %lf\n",i,force[i][0],force[i][1]);
  printf("force[%d]=%lf %lf\n",0,force[0][0],force[0][1]);

  free(force); 
}
