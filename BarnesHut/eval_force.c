#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "BarnesHut.h"


void eval_force(double theta, int n, Particle p[n], Vec2* force)
{
  int id,nt,i_hi,i_lo; // OpemMP
  int i;
  Cell* head = new_cell(); 

  head->size = 1.0;
  head->offset[0] = 0.0;
  head->offset[1] = 0.0;

  into_quadrants(n,p,head);
  get_centre_of_mass(head);

#pragma omp parallel shared(head,p,n,force) private(id,nt,i,i_hi,i_lo)
{
  id=omp_get_thread_num();
  nt=omp_get_num_threads(); 

  i_lo=(id*n)/nt;
  i_hi=((id+1)*n)/nt;
  // printf("%d %d [%d %d]\n",id,nt,i_lo,i_hi);
  if (i_hi>n) i_hi=n;
  
  for (i=i_lo;i<i_hi;i++) 
  {
    add_force(theta, i, p[i].x, head, force[i]); 
  }
}
  
  // for (i=0;i<n;i++) 
    // printf("force[%d]=%lf %lf\n",i,force[i][0],force[i][1]);
  printf("force[%d]=%lf %lf\n",0,force[0][0],force[0][1]);

  free_tree(head);
  free(force); 
}
