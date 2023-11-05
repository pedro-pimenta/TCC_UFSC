#ifndef Mesh_
#define Mesh_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "msg.h"
#include "point.h"
#include "BoundaryEdge.h"
#include "triangle.h"
#include "treefunction.h"
#include "BoundaryCondition.h"

/**************************************************/
/*Inicio da estrutura para o controle da Malha */
/* Esta estrutura usa points, Elements e Boundary Edge */
/*************************************************/

typedef struct Mesh{
   unsigned int NP;         /*NP - Numero total de nodes*/
   point_t** Points;
   unsigned int NE;         /*NE - Numero total de elementos */
   Element_t** Elements;
   unsigned int NBE;         /*NBE = Numero de BoundaryEdge */
   BoundaryEdge_t** BE;     /*vetor de arestas na fronteira*/
   BoundaryCondition_t *BC; /* Controlador de condicoes de fronteira*/
   double GreaterE;         /* Maior aresta */
   double GreaterArea;

}Mesh_t;


/* metodos  implementados */
Mesh_t *new_Mesh();
void libera_Mesh(Mesh_t *M);
point_t* get_Meshpoint(Mesh_t *M, unsigned int index);
Element_t* get_MeshElement(Mesh_t *M, unsigned int index);
BoundaryEdge_t* get_MeshBE(Mesh_t *M, unsigned int index);
Mesh_t *setmesh_BCfunc(Mesh_t *M,unsigned int NFC, function_t *fn, int typeDN);
int get_meshBCtype(Mesh_t *M,unsigned int NFC);
double eval_meshBCfunc(Mesh_t *M,unsigned int NFC, double x, double y);
Mesh_t* read_Mesh(FILE *input);
void write_Mesh(Mesh_t *M, FILE *output);

#endif

