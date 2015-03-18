#include "particle.h" 

// Initialise
Cell* new_cell(); 
void get_centre_of_mass(Cell* c); 
void into_quadrants(int n, Particle* p, Cell *c); 
void free_tree(Cell* c); 
void print(Cell* head); 
void add_force(int k, Vec2 x_k, Cell* c, Vec2 force); 
void eval_force(int n, Particle p[n], Vec2* force); 

extern double theta;
