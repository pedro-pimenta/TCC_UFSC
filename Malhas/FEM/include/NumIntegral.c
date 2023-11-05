#include "NumIntegral.h"

double integralNewman(Mesh_t *M,unsigned int indexBE, double *abc, unsigned int degreeQ){ /*i = indexBE */
point_t *p0, *p1;
double value;
unsigned int k, indexp0, indexp1;
double a,b;
double* W_k, *X_k, X, Y, dXY;
a = 0.;  b= 1.;
W_k = (double *) malloc (degreeQ*sizeof(double ));
X_k = (double *) malloc (degreeQ*sizeof(double ));

   if(degreeQ == 3) {
      W_k[0] = 5./9.*(b-a)*0.5;   X_k[0] = (-sqrt(3./5.)*(b-a)+(b+a))*0.5;
      W_k[1] = 8./9.*(b-a)*0.5;   X_k[1] = (b+a)*0.5;
      W_k[2] = 5./9.*(b-a)*0.5;   X_k[2] = (sqrt(3./5.)*(b-a)+(b+a))*0.5;
   } else {
         degreeQ = 4;
         W_k[0] = (0.5-sqrt(5./6.)/6.)* (b-a)/2.;								  /*peso 0*/
         X_k[0] = (-0.5*sqrt((3.+2.*sqrt(6./5.))/7.)*(b-a)+0.5*(b+a));		      /*ponto 0*/
         W_k[1] = (18.+sqrt(30.))/36.* (b-a)/2.;								  /*peso 1*/
         X_k[1] = -0.5*sqrt((3.-2.*sqrt(6./5.))/7.)*(b-a)+0.5*(b+a);			  /*ponto 1*/
         W_k[2] = (18.+sqrt(30.))/36.* (b-a)/2.;								  /*peso 2*/
         X_k[2] = 0.5*sqrt((3.-2.*sqrt(6./5.))/7.)*(b-a)+0.5*(b+a);			      /*ponto 2*/
         W_k[3] = (0.5-sqrt(5./6.)/6.)* (b-a)/2.;								   /*peso 3*/
         X_k[3] = (0.5*sqrt((3.+2.*sqrt(6./5.))/7.)*(b-a)+0.5*(b+a));			   /*ponto 3*/
   }
   indexp0 = M->BE[indexBE]->N1;
   indexp1 = M->BE[indexBE]->N2;
   p0 = get_Meshpoint(M, indexp0);
   p1 = get_Meshpoint(M, indexp1);
   dXY = sqrt((p0->x-p1->x)*(p0->x-p1->x)+(p0->y-p1->y)*(p0->y-p1->y));
   value = 0.;

   for(k=0;k<degreeQ;k++){
      X = p0->x + (p1->x-p0->x)*X_k[k];
      Y = p0->y + (p1->y-p0->y)*X_k[k];
      value += W_k[k]*eval_meshBCfunc(M,M->BE[indexBE]->BC,X,Y)*(abc[0]+abc[1]*X+abc[2]*Y);
   }
   value *= dXY;

   libera_point(p0);
   libera_point(p1);
   free(W_k);  free(X_k);
   return value;
}


double integralTriangle(Mesh_t *M, function_t * f,unsigned int indexE, double *abc, unsigned int degreeQ){
point_t *p0, *p1, *p2;
double value;
unsigned int k, indexp0, indexp1, indexp2;
double* W_k, *X_k, *Y_k, X, Y, J;

W_k = (double *) malloc (degreeQ*sizeof(double ));
X_k = (double *) malloc (degreeQ*sizeof(double ));
Y_k = (double *) malloc (degreeQ*sizeof(double ));

   if(degreeQ == 3) {
      W_k[0] = 1./3.;   X_k[0] = 0.5;  Y_k[0] = 0.5;  /* exacta ate 3*/
      W_k[1] = 1./3.;   X_k[1] = 0.5;  Y_k[1] = 0.;
      W_k[2] = 1./3.;   X_k[2] = 0.;  Y_k[2] = 0.5;
   } else {
         degreeQ = 4;
         W_k[0] = -9./16.;   X_k[0] = 1./3.;  Y_k[0] = 1./3.;
         W_k[1] = 25./48.;   X_k[1] = 1./5.;  Y_k[1] = 1./5.;
         W_k[2] = 25./48.;   X_k[2] = 1./5.;  Y_k[2] = 3./5.;
         W_k[3] = 25./48.;   X_k[3] = 3./5.;  Y_k[3] = 1./5.;
   }
   indexp0 = M->Elements[indexE]->N1;
   indexp1 = M->Elements[indexE]->N2;
   indexp2 = M->Elements[indexE]->N3;
   p0 = get_Meshpoint(M, indexp0);
   p1 = get_Meshpoint(M, indexp1);
   p2 = get_Meshpoint(M, indexp2);
   J = fabs(p0->x*(p1->y - p2->y) +  p1->x*(p2->y - p0->y) + p2->x*(p0->y - p1->y));
   value = 0.;

   for(k=0;k<degreeQ;k++){
      X = p0->x + (p1->x-p0->x)*X_k[k] + (p2->x-p0->x)*Y_k[k];
      Y = p0->y + (p1->y-p0->y)*X_k[k] + (p2->y-p0->y)*Y_k[k];

      value += W_k[k]* f->fn(X,Y) *(abc[0]+abc[1]*X+abc[2]*Y);
   }
   value *= J*0.5;

   libera_point(p0);
   libera_point(p1);
   libera_point(p2);
   free(W_k);  free(X_k);  free(Y_k);
   return value;

}


double integralL2erroT(Mesh_t *M, function_t * f, double *aprox_sol, unsigned int indexE, basis_t *phi, unsigned int degreeQ){
point_t *p0, *p1, *p2;
double value, total_value;
unsigned int k, indexp0, indexp1, indexp2;
double* W_k, *X_k, *Y_k, X, Y, J;

W_k = (double *) malloc (degreeQ*sizeof(double ));
X_k = (double *) malloc (degreeQ*sizeof(double ));
Y_k = (double *) malloc (degreeQ*sizeof(double ));

   if(degreeQ == 3) {
      W_k[0] = 1./3.;   X_k[0] = 0.5;  Y_k[0] = 0.5;  /* exacta ate 3*/
      W_k[1] = 1./3.;   X_k[1] = 0.5;  Y_k[1] = 0.;
      W_k[2] = 1./3.;   X_k[2] = 0.;  Y_k[2] = 0.5;
   } else {
         degreeQ = 4;
         W_k[0] = -9./16.;   X_k[0] = 1./3.;  Y_k[0] = 1./3.;
         W_k[1] = 25./48.;   X_k[1] = 1./5.;  Y_k[1] = 1./5.;
         W_k[2] = 25./48.;   X_k[2] = 1./5.;  Y_k[2] = 3./5.;
         W_k[3] = 25./48.;   X_k[3] = 3./5.;  Y_k[3] = 1./5.;
   }
   indexp0 = M->Elements[indexE]->N1;
   indexp1 = M->Elements[indexE]->N2;
   indexp2 = M->Elements[indexE]->N3;
   p0 = get_Meshpoint(M, indexp0);
   p1 = get_Meshpoint(M, indexp1);
   p2 = get_Meshpoint(M, indexp2);
   J = fabs(p0->x*(p1->y - p2->y) +  p1->x*(p2->y - p0->y) + p2->x*(p0->y - p1->y));
   total_value = 0.;

   for(k=0;k<degreeQ;k++){
      X = p0->x + (p1->x-p0->x)*X_k[k] + (p2->x-p0->x)*Y_k[k];
      Y = p0->y + (p1->y-p0->y)*X_k[k] + (p2->y-p0->y)*Y_k[k];

      value = W_k[k]*( aprox_sol[indexp0]*(phi->abc[0][0]+phi->abc[0][1]*X+phi->abc[0][2]*Y) + 
                        aprox_sol[indexp1]*(phi->abc[1][0]+phi->abc[1][1]*X+phi->abc[1][2]*Y) +
                        aprox_sol[indexp2]*(phi->abc[2][0]+phi->abc[2][1]*X+phi->abc[2][2]*Y) -
                        f->fn(X,Y) );

      total_value += value*value;
   }
   total_value *= J*0.5;

   libera_point(p0);
   libera_point(p1);
   libera_point(p2);
   free(W_k);  free(X_k);  free(Y_k);
   return total_value;


}

