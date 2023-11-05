#ifndef Point_
#define Point_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "msg.h"

/**************************************************/
/*Inicio da estrutura para pontos */
/*************************************************/

typedef struct point{
   double x;
   double y;
}point_t;


/* metodos  implementados */
point_t *new_point(double x, double y);
void libera_point(point_t *p);
double get_x(point_t* p);
double get_y(point_t* p);
void print_point(point_t* p);


#endif
