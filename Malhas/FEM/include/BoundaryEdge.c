#include "BoundaryEdge.h"

/**************************************************/
/*Inicio da estrutura para as condicoeses de fronteira*/
/*************************************************/


/*Cria um apontador de BoundaryEdge, com os indices N1, N2 que ligam a uma lista de pontos e a respetiva condi��o de fronteira.
BC = -1 � condi��o de Dirichlet, BC = 0 � condi��o de Newman.
A arista � dada pelo segmento de reta p0,p1 e N1 liga com p0 e N2 liga com p1.
� recomendado que a orienta��o dos vertices sejam num sentido s�, horario o antihorario.*/
BoundaryEdge_t *new_BoundaryEdge(unsigned int N1,unsigned int N2,unsigned int NE, int BC){
   BoundaryEdge_t *BE;

   BE = (BoundaryEdge_t *) malloc (sizeof(BoundaryEdge_t));
   if(!BE) problem();
   BE->N1 = N1;
   BE->N2 = N2;
   BE->NE = NE;
   BE->BC = BC;

   return BE;
}

void libera_BoundaryEdge(BoundaryEdge_t *BE){
   if(BE){
      free(BE);
   }
}

/* Retorna o indice 1 de BE*/
unsigned int get_BoundaryEdgeN1(BoundaryEdge_t *BE){
   return BE->N1;
}

/* Retorna o indice 2 de BE*/
unsigned int get_BoundaryEdgeN2(BoundaryEdge_t *BE){
   return BE->N2;
}

/* Retorna o indice do elemento ao qual pertence*/
unsigned int get_BoundaryNumElement(BoundaryEdge_t *BE){
   return BE->NE;
}

/* Retorna a condi��o de fronteira de BE*/
int get_BoundaryEdgeBC(BoundaryEdge_t *BE){
   return BE->BC;
}

/* Imprime os dados de BE*/
void print_BoundaryEdge(BoundaryEdge_t* BE){
   printf(" %d %d %d %d\n",BE->N1, BE->N2, BE->NE, BE->BC);
}
