#ifndef FEM_
#define FEM_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "msg.h"
#include "CSRmat.h"
#include "point.h"
#include "BoundaryEdge.h"
#include "triangle.h"
#include "Mesh.h"
#include "NumIntegral.h"
#include "basis.h"

/**************************************************/
/*Inicio da estrutura de Elementos Finitos */
/* Esta estrutura tem uma matriz esparsa MatCSR,  */
/*tem uma malha Mesh, Armazena as bases para cada */
/*elemento basis_t e tem un vetor 'rhs' para o    */
/*sistema de elementos finitos, finalmente um vetor*/
/*gama com os coeficientes da solucao aproximada*/
/*************************************************/

typedef struct FEM{
   Mesh_t *M;                /*Malha*/
   MatCSR_t *A;              /*Matriz esparsa */
   int* newindexP;           /*New index to points*/
   int NPinte_newman;        /*Quantidade de pontos interiores ou Newman*/
   double *rhs;              /*Vetor rhs */
   double *gama;             /*Vetor de constantes para a combinacao linear*/
   basis_t **phi;            /*Vetor de bases por elemento */
   function_t* f;            /*funcao no rhs da equacao de Poisson */
   int flag;                 /*Indica se há convergencia ou nao*/
}FEM_t;

/* metodos  implementados */
FEM_t* new_FEM(Mesh_t *M, function_t *fn);
FEM_t *set_systemFEM(FEM_t *fem);
void write_noderro(FEM_t *fem,function_t *Sol,FILE *output);
void print_info(FEM_t *fem,function_t *Sol);
double get_maxerro(FEM_t *fem,function_t *Sol);
double get_erroL2(FEM_t *fem,function_t *Sol);

#endif

