typedef double Vec2[2]; 

typedef struct
{
  double mass;
  Vec2	 x;
}
Particle; 

typedef struct Cell_struct
{
  double		size;
  Vec2			offset;
  Particle 		particle;
  struct Cell_struct*	quad[4];
}
Cell;
