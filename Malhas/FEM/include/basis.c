#include "basis.h"

/**************************************************/
/*Inicio da estrutura para bases por elemento */
/* abc[0] => a_0 + b_0 x + c_0 y*/
/* abc[1] => a_1 + b_1 x + c_1 y*/
/* abc[2] => a_2 + b_2 x + c_2 y*/
/* Tambem armazena a area do elemento*/
/*************************************************/


basis_t *new_basis(){
int i;
basis_t* phi;
   phi = (basis_t *) malloc (sizeof(basis_t));
   if(!phi) problem();
   phi->abc = (double **) malloc (3*sizeof(double*));
   for(i=0;i<3;i++){
      phi->abc[i] = (double *) malloc (3*sizeof(double));
      phi->abc[i][0] = 0.;
      phi->abc[i][1] = 0.;
      phi->abc[i][2] = 0.;
   }
   phi->area = 0.;
return phi;
}

/* Evalua o ponto p(x,y) na base phi_j*/
double eval_phi(basis_t* phi,unsigned int j,double x,double y){
double *ptr_abc;
   if(j<0 || j>=3){
      printf("ERRO::eval_phi:Index basis not valid\n");
      exit(1);
   }
   ptr_abc = phi->abc[j];

   return ptr_abc[0] + ptr_abc[1]*x + ptr_abc[2]*y;  /* a_j+b_x+c_jy*/
}

/* Gera os tres elementos de base canonica segundo o triangulo de vertices p0,p1 e p2.  Lembrar abc[i] => a_i + b_i x + c_i y*/
basis_t *set_basis(basis_t *phi, point_t *p0, point_t *p1, point_t *p2){
double *ptr_abc;
double Det;

   Det = (p1->x*p2->y - p2->x*p1->y) - p0->x*(p2->y - p1->y) + p0->y*(p2->x - p1->x);
   
   phi->area = fabs(Det *0.5);
   
   ptr_abc = phi->abc[0];
   ptr_abc[0] = (p1->x*p2->y - p1->y*p2->x)/Det;
   ptr_abc[1] = (p1->y - p2->y)/Det;
   ptr_abc[2] = (p2->x - p1->x)/Det;
   
   ptr_abc = phi->abc[1];
   ptr_abc[0] = (p2->x*p0->y - p2->y*p0->x)/Det;
   ptr_abc[1] = (p2->y - p0->y)/Det;
   ptr_abc[2] = (p0->x - p2->x)/Det;
   
   ptr_abc = phi->abc[2];
   ptr_abc[0] = (p0->x*p1->y - p0->y*p1->x)/Det;
   ptr_abc[1] = (p0->y - p1->y)/Det;
   ptr_abc[2] = (p1->x - p0->x)/Det;
   
   return phi;
}

/* funcao para imprimir na tela a base phi_i*/
void print_basis(basis_t *phi, unsigned int i){
double *ptr_abc;

   if(i>2 || i<0){
      printf("ERRO::print_basis:Index for basis not valid\n");
      exit(1);
   }

   ptr_abc = phi->abc[i];
   printf("%.4f ",ptr_abc[0]);
   if(ptr_abc[1] >= 0.0)   printf("+%.4f x ",ptr_abc[1]);
   else printf("%.4f x ",ptr_abc[1]);
   if(ptr_abc[2] >= 0.0)   printf("+%.4f y",ptr_abc[2]);
   else printf("%.4f y",ptr_abc[2]);
}
