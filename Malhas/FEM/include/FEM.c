#include "FEM.h"


double TOLF = 1e-8;

/**************************************************/
/*Inicio da estrutura de Elementos Finitos       */
/* Esta estrutura tem uma matriz esparsa MatCSR,  */
/*tem uma malha Mesh, Armazena as bases para cada */
/*elemento basis_t e tem un vetor 'rhs' para o    */
/*sistema de elementos finitos, finalmente um     */
/*vetor gama com os coeficientes da solucao aproximada */
/*************************************************/


FEM_t* new_FEM(Mesh_t *M, function_t *fn){
Element_t *T;
point_t *p0, *p1, *p2;
FEM_t* fem;
int i, N1, N2, NP, NE, NBE;
int cont_NewmanP, cont_DirichletP;

   fem = (FEM_t *) malloc (sizeof(FEM_t));
   if(!fem) problem();

   NE = M->NE;
   NBE = M->NBE;
   NP = M->NP;
   fem->M = M;
   fem->f = fn;
   fem->newindexP = (int *) malloc (NP*sizeof(int ));

   for(i=0;i<NP;i++)   fem->newindexP[0] = 0;

   for(i=0;i<NBE;i++){
      N1 = M->BE[i]->N1;
      N2 = M->BE[i]->N2;
      if(get_meshBCtype(M,M->BE[i]->BC)==1){
            fem->newindexP[N1] = -1;
            fem->newindexP[N2] = -1;
         }
   }
   cont_NewmanP = 0, cont_DirichletP = NP-1;
   for(i=0;i<NP;i++){
      if(!fem->newindexP[i])   fem->newindexP[i] = cont_NewmanP++;
   }
   fem->NPinte_newman = cont_NewmanP;

   /* Alocamento de memoria dos vetores e da matriz*/
   fem->rhs = (double *) malloc (fem->NPinte_newman*sizeof(double ));
   fem->gama = (double *) malloc (M->NP*sizeof(double ));
   for(i=0;i<cont_NewmanP;i++) fem->rhs[i] = 0.;
   fem->A = new_matrix(cont_NewmanP,cont_NewmanP);

   /*Criacao das bases para cada elemento */
   fem->phi = (basis_t **) malloc (NE*sizeof(basis_t* ));
   for(i=0;i<NE;i++){
      fem->phi[i] = new_basis();
      T = get_MeshElement(M, i);
      p0 = get_Meshpoint(M, get_ElementN1(T));
      p1 = get_Meshpoint(M, get_ElementN2(T));
      p2 = get_Meshpoint(M, get_ElementN3(T));
      fem->phi[i] = set_basis(fem->phi[i],p0,p1,p2);
      libera_Element(T);
      libera_point(p0);
      libera_point(p1);
      libera_point(p2);
   }
   return fem;
}



/*Seguindo o algoritmo 12.5 do exercicio proposto, criamos a matriz e o veto para solucionar o sistema, depois com o uso de Gradiente Conjugado obtemos os coeficentes que estavao faltando e finalmente conseguimos os valores gamma da combinacao linear que aproxima a solucao*/
FEM_t *set_systemFEM(FEM_t *fem){
int i, j, k, NE;
int EN[3], l, t;
basis_t *ptr_phi;
MatCSR_t *Z, *J;  /*local Matrix by element */
double H[3], I[3];
double value;
Element_t *T;
unsigned int edge;
double* sol;


   J = new_matrix(3,3);
   /*Set dado pela condicao de fronteira de Newman e Dirichlet segundo o caso*/
   for(edge=0;edge<fem->M->NBE;edge++){
      if(get_meshBCtype(fem->M,fem->M->BE[edge]->BC)==-1){ /*Aresta Newman*/
         i = fem->M->BE[edge]->NE;  /*elemento */
         ptr_phi = fem->phi[i];
         T = get_MeshElement(fem->M,i);
         EN[0] = get_ElementN1(T);
         EN[1] = get_ElementN2(T);
         EN[2] = get_ElementN3(T);
      
         for(j=0;j<3;j++){
            I[j] = integralNewman(fem->M,edge,fem->phi[i]->abc[j],3); /*Integral em linha*/
         }
         for(k=0;k<3;k++){
            l = fem->newindexP[EN[k]];  
            for(j=0;j<k;j++){
               t = fem->newindexP[EN[j]];
               if(l != -1){                          /*Nao é Dirichlet*/
                  if(t != -1){                       /*Nao é Dirichlet*/
                     fem->A = add_aij(fem->A,l,t,get_aij(J,k,j));
                     fem->A = add_aij(fem->A,t,l,get_aij(J,k,j));
                  } else   fem->rhs[l] = fem->rhs[l] - fem->gama[EN[j]]*get_aij(J,k,j);   /*é Dirichlet */
               } else {                              /*Dirichlet */
                     if(t != -1)  fem->rhs[t] = fem->rhs[t] - fem->gama[EN[k]]*get_aij(J,k,j);
               }
            } /*Fecha for para j*/ 

            if(l != -1){                              /*Nao é Dirichler*/
               fem->A = add_aij(fem->A,l,l,get_aij(J,k,k));
               fem->rhs[l] = fem->rhs[l] + I[k];
            } 
         } /*Fecha for para k*/
      
         libera_Element(T);
         
      } else {                                       /*Aresta Dirichlet */
            EN[0] = fem->M->BE[edge]->N1;
            EN[1] = fem->M->BE[edge]->N2;
            /* Valor exato dado pela funcao correspondente */            
            fem->gama[EN[0]] = eval_meshBCfunc(fem->M,fem->M->BE[edge]->BC, fem->M->Points[EN[0]]->x, fem->M->Points[EN[0]]->y);
            fem->gama[EN[1]] = eval_meshBCfunc(fem->M,fem->M->BE[edge]->BC, fem->M->Points[EN[1]]->x, fem->M->Points[EN[1]]->y);
        }
   } /*Fecha for para edge*/


   NE = fem->M->NE;
   Z = new_matrix(3,3);
   /* percurso de todos os elementos triangulares */
   for(i=0;i<NE;i++){
      ptr_phi = fem->phi[i];
      T = get_MeshElement(fem->M,i);
      for(j=0;j<3;j++){
         for(k=0;k<=j;k++){
            value = ptr_phi->abc[j][1]*ptr_phi->abc[k][1]*ptr_phi->area + ptr_phi->abc[j][2]*ptr_phi->abc[k][2]*ptr_phi->area;
            Z = set_aij(Z,j,k,value);
         }

         H[j] = -integralTriangle(fem->M, fem->f,i,fem->phi[i]->abc[j],3);  /*Integral triangular*/
      }
   
      /* Aca bem uma parte que inclui integrais em linha, nosso casso sao zero e nao implemente*/
      EN[0] = get_ElementN1(T);
      EN[1] = get_ElementN2(T);
      EN[2] = get_ElementN3(T);
      for(k=0;k<3;k++){
         l = fem->newindexP[EN[k]];
            for(j=0;j<k;j++){
               t = fem->newindexP[EN[j]];
               if(l != -1){                             /*Nao é Dirichlet */
                  if(t != -1){                          /*Nao é Dirichlet */
                     fem->A = add_aij(fem->A,l,t,get_aij(Z,k,j));
                     fem->A = add_aij(fem->A,t,l,get_aij(Z,k,j));
                  } else   fem->rhs[l] = fem->rhs[l] - fem->gama[EN[j]]*get_aij(Z,k,j);  /*Dirichlet, fem->gama[t]*/
               } else {                                 /*é Dirichlet */
                     if(t!= -1)                        /*Nao é Dirichlet */
                        fem->rhs[t] = fem->rhs[t] - fem->gama[EN[k]]*get_aij(Z,k,j);
                 }
            } /*Fecha for para j*/

         if(l != -1){                                  /*Nao é Dirichlet */
            fem->A = add_aij(fem->A,l,l,get_aij(Z,k,k));
            fem->rhs[l] = fem->rhs[l] + H[k];
         } 
      } /*Fecha for para k*/
   
      libera_Element(T);
   } /*Fecha for para i---> NE*/


   /*Solucao do sistema A sol = rhs*/
   sol = (double *) malloc (fem->NPinte_newman*sizeof(double ));
   fem->flag = CSR_CG(fem->A, fem->rhs, fem->rhs, fem->M->NP + (fem->M->NP*0.2), TOLF, sol);  /*Solver*/
   if(fem->flag){ /* converge */
      for(i=0;i<fem->M->NP;i++){
         if(fem->newindexP[i] != -1)  fem->gama[i] = sol[fem->newindexP[i]];
      }
   } else printf("System does not solutioned\n");
   free(sol);

   return fem;
}


void write_noderro(FEM_t *fem,function_t *Sol,FILE *output){
unsigned int i, NP;
point_t *p;
double value, erro;

   NP = fem->M->NP;

   fprintf(output,"\nx\t\ty\t\tphi(x,y)\t\tU(x,y)\t\t|phi(x,y) - U(x,y)|\n");
   for(i=0;i<NP;i++){
      p = get_Meshpoint(fem->M, i);
      value = Sol->fn(p->x,p->y);
      erro = fabs(value-fem->gama[i]);
      fprintf(output,"%.8f\t%.8f\t%.8f\t\t%.8f\t\t%.12f\n",p->x,p->y,fem->gama[i],value,erro);
      libera_point(p);
   }

}


/* Imprime na tela a informacao obtida com o metodo, matriz, vetor rhs, vetor solucao, bases, combinacao linear e erro ponto por ponto*/
void print_info(FEM_t *fem,function_t *Sol){
unsigned int i, k, NP, NE, EN[3];
point_t *p;
basis_t *ptr_phi;
Element_t *T;
double value, erro, maxerro = 0.;

   NP = fem->M->NP;
   NE = fem->M->NE;

   printf("Generate Matrix A = \n");
   imprime_matrix(fem->A);

   printf("\nGenerate Vector b = \n");
   for(i=0;i<NP;i++)   
      if(fem->newindexP[i] !=-1) printf("b[%d] = %f\n",i,fem->rhs[fem->newindexP[i]]);

   if(fem->flag){ /* converge */
      printf("\nThe solution to the equation is Ac = b\n");
      for(i=0;i<fem->M->NP;i++)  printf("c[%d] = %f\n",i,fem->gama[i]);

      printf("\nApproximation on respective triangles :\n");

      for(i=0;i<NE;i++){
         ptr_phi = fem->phi[i];
         T = get_MeshElement(fem->M,i);
         EN[0] = get_ElementN1(T);
         EN[1] = get_ElementN2(T);
         EN[2] = get_ElementN3(T);
         printf("T_%d : phi(x,y) = ",i);
   
         printf("%.4f (",fem->gama[EN[0]]);
         print_basis(ptr_phi,0);
         printf(")");
         for(k=1;k<3;k++){
            if(fem->gama[EN[k]] >= 0.0) printf("+%.4f (",fem->gama[EN[k]]);
            print_basis(ptr_phi,k);
            printf(")");
         }
         printf("\n");
      }

   } else printf("System does not solutioned\n");

   printf("\nx\t\ty\t\tphi(x,y)\t\tu(x,y)\t\t|phi(x,y) - u(x,y)|\n");
   for(i=0;i<NP;i++){
      p = get_Meshpoint(fem->M, i);
      value = Sol->fn(p->x,p->y);
      erro = fabs(value-fem->gama[i]);
      if (erro > maxerro)  maxerro = erro;
      printf("%.1f\t%.1f\t%f\t\t%f\t\t%f\n",p->x,p->y,fem->gama[i],value,erro);
      libera_point(p);
   }

   printf("Erro max = %f\n",maxerro);
}

/*retorna o maior erro ponto por ponto */
double get_maxerro(FEM_t *fem, function_t *Sol){
unsigned int i, NP;
point_t *p;
double value, erro, maxerro = 0.;

   NP = fem->M->NP;
   for(i=0;i<NP;i++){
      p = get_Meshpoint(fem->M, i);
      value = Sol->fn(p->x,p->y);
      erro = fabs(value-fem->gama[i]);
      if (erro > maxerro)  maxerro = erro;
      libera_point(p);
   }

   printf("Erro max = %1.10f\n",maxerro);
   return maxerro;
}

/*retorna o erro com a norma L2*/
double get_erroL2(FEM_t *fem,function_t *Sol){
unsigned int i, index_maxerro, NE;
double erro = 0., local_erro, maxerro = 0.;

   NE = fem->M->NE;
   index_maxerro = NE;

   for(i=0;i<NE;i++){
      local_erro = integralL2erroT(fem->M, Sol, fem->gama, i, fem->phi[i], 3);
      erro += local_erro;
      if(local_erro > maxerro){ 
         maxerro = local_erro;
         index_maxerro = i;
      }

    }

   erro = sqrt(erro);
   printf("Errol2 = %1.10f e o maxl2erro foi para o elemento %d v0\n",erro,index_maxerro);
   print_point(fem->M->Points[fem->M->Elements[index_maxerro]->N1]);
   printf(" v1");
   print_point(fem->M->Points[fem->M->Elements[index_maxerro]->N2]);
   printf(" e v2");
   print_point(fem->M->Points[fem->M->Elements[index_maxerro]->N3]);
   printf("\n");
   return  erro;
}


