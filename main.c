#include<stdio.h>
#include<stdlib.h>
#include<time.h>
//#include<mpi.h>
#define pi 3.14159265359

void leap_frog(double *theta, double *omega, double *omega_bar, int num_cars);

int main(){

  int num_cars, i;
  double *theta, *omega, *omega_bar, a, diff, r;

  num_cars=10;
  a=0.1;
  srand48(time(NULL));

  theta=(double *)malloc(sizeof(double)*num_cars);
  omega=(double *)malloc(sizeof(double)*num_cars);
  omega_bar=(double *)malloc(sizeof(double)*num_cars);

  diff=(2*pi)/((double)num_cars);

  //Initialise theta, omega and omega_bar

  theta[0]=0;
  for(i=1;i<num_cars;i++){
    theta[i]=theta[i-1]+diff;
  }

  i=0;
  while(i<num_cars){
    r=drand48();//random number
    if(r>a){//bigger than the min speed
      omega_bar[i]=r;
      //printf("%f\n", omega_bar[i]);
      i++;
    }
  }

  for(i=0;i<num_cars;i++)
    omega[i]=0;//set initial omega 

  leap_frog(theta, omega, omega_bar, num_cars);

  printf("%f\n", theta[0]);

}
