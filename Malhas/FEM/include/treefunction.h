
#ifndef FN_
#define FN_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "msg.h"

/*******************************************************/
/*Estrutura que controla a inicializacao de funcoes por*/ 
/*meio de aquivos externos                             */
/*******************************************************/

typedef struct function{
   double (*fn)(double,double);
} function_t;

/*******************************************************/
/* Estrutura que controla o armazenamento de funcoes   */
/* identificadas com um indice f_i                     */
/*******************************************************/
typedef struct balance_treefn{
   unsigned int index;     /*function's index*/
   function_t* fn;         /*function stored*/
   int colour;             /*Colour for Rubro-Negra node*/

   struct balance_treefn* Left;  /* pointer to left tree*/
   struct balance_treefn* Right; /* pointer to right tree*/
} treefn_t;


/* metodos  implementados */
function_t *asigned_function(double (*fn)(double,double));
treefn_t *new_treefn();
treefn_t *new_node_treefn(treefn_t *F,unsigned int index_fn, function_t *fn);
void imprime_Inordem_indextreefn(treefn_t *F);
treefn_t *RotacioneEsqtreefn(treefn_t *F);
treefn_t *RotacioneDirtreefn(treefn_t *F);
treefn_t *TroqueCorestreefn(treefn_t *F);
function_t *buscatreefn(treefn_t *F,unsigned int j);
double evaltreefn(treefn_t *F,unsigned int j, double x, double y);
treefn_t *insere_Rectreefn(treefn_t *F,unsigned int j,function_t *fn);
treefn_t *insere_treefn(treefn_t* F, unsigned int j, function_t * fn);

#endif
