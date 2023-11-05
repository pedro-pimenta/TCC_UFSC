#include "point.h"

/**************************************************/
/*Inicio da estrutura para pontos */
/*************************************************/

/*Cria um ponto p(x,y)*/
point_t *new_point(double x, double y){
   point_t *p;

   p = (point_t *) malloc (sizeof(point_t));
   if(!p) problem();
   p->x = x;
   p->y = y;
   return p;
}

void libera_point(point_t *p){
   if(p){
      free(p);
   }
}

/*Retorna a coordenada x do ponto p*/
double get_x(point_t* p){
   return p->x;
}

/*Retorna a coordenada y do ponto p*/
double get_y(point_t* p){
   return p->y;
}

/*Imprime um ponto p*/
void print_point(point_t* p){
   printf(" (%.6f,%.6f) ",p->x,p->y);
}
