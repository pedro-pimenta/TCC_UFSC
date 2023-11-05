#ifndef File_
#define File_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "msg.h"
#include "BoundaryEdge.h"
#include "triangle.h"


typedef struct Elemno{
   Element_t * T;             /* Triangle Element to list */
   struct Elemno * prox;
}Elemno_t;


typedef struct ElementList{
   int num;                   /*quantity of element of the element list*/
   Elemno_t * begin;
   Elemno_t * end;
} list_t;

typedef struct BEno{
   BoundaryEdge_t * BE;         /* Boundary Edge to list */
   struct BEno * prox;
}BEno_t;

typedef struct BoundaryEdgeList{
   int num;                   /*quantity of edges of the BoundaryEdges list*/
   int numBC;
   BEno_t * begin;
   BEno_t * end;
} BElist_t;


Elemno_t *new_Elemno(unsigned int N1, unsigned int N2, unsigned int N3);
void libera_Elemno(Elemno_t *n);
list_t * new_ElementList();
list_t *insertElemList(list_t * L, unsigned int N1,unsigned int N2,unsigned int N3);
void imprimeListFile(FILE *output, list_t * f);
void libera_List(list_t * L);
BEno_t *new_BEno(unsigned int N1, unsigned int N2, unsigned int NE, int BC);
void libera_BEno(BEno_t *n);
BElist_t * new_BoundaryEdgeList();
BElist_t *insertBEList(BElist_t * L, unsigned int N1,unsigned int N2,unsigned int NE, int BC);
void imprimeBEListFile(FILE *output,BElist_t *L);
BElist_t *set_BEinElement(BElist_t *BEL,list_t *TL);
void libera_BEList(BElist_t * L);
void readGMSH_writeMesh(FILE *input,FILE *output);

#endif
