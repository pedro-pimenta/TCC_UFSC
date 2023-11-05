#ifndef CSR_
#define CSR_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "msg.h"

/**************************************************/
/*Inicio da estrutura para Arvores balanceados Rubro Negra */
/*************************************************/

typedef struct balance_tree{
   unsigned int column;          /*column index*/
   double value;                 /*Value not zero stored*/
   int colour;                   /*Colour for Rubro-Negra node*/

   struct balance_tree* Left;  /* pointer to left tree*/
   struct balance_tree* Right; /* pointer to right tree*/
} RowCSR_t;


/**************************************************/
/*Inicio da estrutura para Matriz esparsa */
/* Formato de armazenamento CSR */
/*************************************************/

typedef struct MatCSR{
   unsigned int n;
   unsigned int m;
   RowCSR_t** Rows;
} MatCSR_t;


/* metodos  implementados */
RowCSR_t *new_tree();
RowCSR_t *new_node_tree(RowCSR_t *T,unsigned int j, double value);
void imprime_nivel(RowCSR_t *T, int i);
void imprime_Inordem(RowCSR_t *T);
void write_MATLAB(RowCSR_t *T,int i, FILE *output); 
int contenos(RowCSR_t *T);
int altura(RowCSR_t *T);
RowCSR_t *RotacioneEsq(RowCSR_t *T);
RowCSR_t *RotacioneDir(RowCSR_t *T);
RowCSR_t *TroqueCores(RowCSR_t *T);
double buscaRN(RowCSR_t *T,unsigned int j);
RowCSR_t *insere_Rec(RowCSR_t *T,unsigned int j,double aij);
RowCSR_t *insere_RN(RowCSR_t* T, unsigned int j, double aij);
RowCSR_t *CSRadd_Rec(RowCSR_t *T,unsigned int j,double aij);
RowCSR_t *CSRadd(RowCSR_t *T,unsigned int j,double aij);
double dot_produc_tree(RowCSR_t *T,double* x);
MatCSR_t *new_matrix(unsigned int n, unsigned int m);
MatCSR_t *set_aij(MatCSR_t* M, unsigned int i, unsigned int j, double aij);
double get_aij(MatCSR_t* M, unsigned int i, unsigned int j);
MatCSR_t *add_aij(MatCSR_t* M, unsigned int i, unsigned int j, double aij);
int get_NNZ(MatCSR_t* M); 
void imprime_matrix(MatCSR_t *M);
void write_MATLABmatrix(MatCSR_t *M,FILE *output); 
void mat_vec(MatCSR_t *M,double* x, double* y);
double ddot(unsigned int dim,double * x1,double* x2);
void daxpy(double a,unsigned int dim,double* x, double* y);
void dcopy(unsigned int dim, double* xCopy, double* x);
unsigned int CSR_CG(MatCSR_t *M, double *b, double *x_0, unsigned int max_iter, double tol, double *X);


#endif
