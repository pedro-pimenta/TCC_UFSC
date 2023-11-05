
#ifndef BC_
#define BC_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "msg.h"
#include "treefunction.h"

/**************************************************/
/*Inicio da estrutura para condicoes de fonteira  */
/* fn é um arbol de funcoes onde ficam armazenadas*/ 
/*para cada numeo de fronteira a funcao assinada  */
/*permitindo busca logaritmica                    */
/*set_DN é um vetor onde cada posicao indica se o */
/*numero de condicao de fonteira correspondente é */
/*Dirichlet ou Newman                             */
/*************************************************/

typedef struct BoundaryCondition{
   treefn_t* fn;
   unsigned int NC;  /*Quantidade de condicoes para as funcoes*/
   int* set_DN;     /*1---> Dirichlet, -1--->Newman, 0 not assigned */
}BoundaryCondition_t;


/* metodos  implementados */
BoundaryCondition_t *new_BoundaryCondition(unsigned int NC);
void libera_BoundaryCondition(BoundaryCondition_t *BC);
int get_BoundaryConditionDN(BoundaryCondition_t *BC,unsigned int NFC);
double eval_Boundaryfunc(BoundaryCondition_t *BC,unsigned int NFC, double x, double y);
BoundaryCondition_t *set_Condition_func(BoundaryCondition_t *BC,unsigned int NFC, function_t *fn, int typeDN);


#endif

