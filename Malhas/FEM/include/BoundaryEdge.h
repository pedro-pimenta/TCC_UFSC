#ifndef BE_
#define BE_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "msg.h"

/*******************************************************/
/*Inicio da estrutura para controlar a relacao entre   */
/*as arestas, as condicoes de fronteira e o elemento ao */
/*qual a aresta pertence                               */
/*******************************************************/

typedef struct BoundaryEdge{
   unsigned int N1;   /*N1 and N2 are Index to point*/
   unsigned int N2;
   unsigned int NE;   /*Index to element*/
   int BC;            /*Number of Boundary condition*/
   
}BoundaryEdge_t;


/* metodos  implementados */
BoundaryEdge_t *new_BoundaryEdge(unsigned int N1,unsigned int N2,unsigned int NE, int BC);
void libera_BoundaryEdge(BoundaryEdge_t *BE);
unsigned int get_BoundaryEdgeN1(BoundaryEdge_t *BE);
unsigned int get_BoundaryEdgeN2(BoundaryEdge_t *BE);
unsigned int get_BoundaryNumElement(BoundaryEdge_t *BE);
int get_BoundaryEdgeBC(BoundaryEdge_t *BE);
void print_BoundaryEdge(BoundaryEdge_t* BE);


#endif
