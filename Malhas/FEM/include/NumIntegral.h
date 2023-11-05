#ifndef Int_
#define Int_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "msg.h"
#include "point.h"
#include "Mesh.h"
#include "basis.h"

/* metodos  implementados 
para o calculo de integrais numericas em linha ou em dominho triangular*/
double integralNewman(Mesh_t *M,unsigned int indexBE, double *abc, unsigned int degreeQ);
double integralTriangle(Mesh_t *M, function_t * f,unsigned int indexE, double *abc, unsigned int degreeQ);
double integralL2erroT(Mesh_t *M, function_t * f, double *aprox_sol, unsigned int indexE, basis_t *phi, unsigned int degreeQ);

#endif

