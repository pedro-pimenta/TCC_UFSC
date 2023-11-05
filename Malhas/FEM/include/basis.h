#ifndef phi_
#define phi_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "msg.h"
#include "point.h"

/**************************************************/
/*Inicio da estrutura para bases por elemento */
/* abc[0] => a_0 + b_0 x + c_0 y*/
/* abc[1] => a_1 + b_1 x + c_1 y*/
/* abc[2] => a_2 + b_2 x + c_2 y*/
/* Tambem armazena a area do elemento*/
/*************************************************/

typedef struct basis{
   double **abc;
   double area;
}basis_t;

/* metodos  implementados */
basis_t *new_basis();
double eval_phi(basis_t* phi,unsigned int j,double x,double y);
basis_t *set_basis(basis_t *phi, point_t *p0, point_t *p1, point_t *p2);
void print_basis(basis_t *phi, unsigned int i);

#endif


