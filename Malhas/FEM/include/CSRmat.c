#include "CSRmat.h"

double TOL = 1e-12;

/**************************************************/
/*Inicio da estrutura para Matriz esparsa        */
/* Formato de armazenamento CSR                  */
/*************************************************/

RowCSR_t *new_tree(){
  RowCSR_t *T;

   T = (RowCSR_t *) malloc (sizeof(RowCSR_t));
   if(!T) problem();
   T->Left = NULL;
   T->Right = NULL;
   T->colour = 0;
      
  return T;
}

RowCSR_t *new_node_tree(RowCSR_t *T,unsigned int j, double value){

   if(j>=0){
      T->column = j;
      T->value = value;
      T->colour = 1;
      T->Left = new_tree();
      T->Right = new_tree();
   }
      
  return T;
}

void imprime_nivel(RowCSR_t *T, int i){  
  if(T->Left || T->Right){
     if(!i){
      printf(" c%d(%d, %f) ",T->colour,T->column,T->value);
     } else {
          imprime_nivel(T->Left,i-1);
          imprime_nivel(T->Right,i-1);
       }
  } else {
       printf("N\t");
    }
}

void imprime_Inordem(RowCSR_t *T){
  if(T->Left || T->Right){
      imprime_Inordem(T->Left);
      printf(" (%d, %f) ",T->column,T->value);
      imprime_Inordem(T->Right);
   }
}


void write_MATLAB(RowCSR_t *T,int i, FILE *output){
 if(T->Left || T->Right){
      write_MATLAB(T->Left,i,output);
      fprintf(output," A(%d, %d)=%f;\n ",i+1,T->column+1,T->value);
      write_MATLAB(T->Right,i,output);
   }
}

/* Retorna a quantidade de n'os do arvore T */
int contenos(RowCSR_t *T){
  if(!T->Left && !T->Right) return 0;
  return 1+contenos(T->Left)+contenos(T->Right);
}

/* Retorna a altura do arvore T */
int altura(RowCSR_t *T){
  int he,h;
  if(!T) return -1;

  he = altura(T->Left);
  h = altura(T->Right);
  if(he>h) h = he;

  return 1+h;
}

/*Rotacao a Esquerda da raiz de T, segundo Rubro Negra pois tambem troca de cor.
Esta funcao 'e usada na insercao RubroNegra*/
RowCSR_t *RotacioneEsq(RowCSR_t *T){
   RowCSR_t *ptr_Q;

   if(T->Right){ /*Arvore direito nao nulo*/
      if(T->Right->Left && T->Right->Right){ /*Arvore esquerdo nao 'e folha*/
         ptr_Q = T->Right;
         T->Right = ptr_Q->Left;
         ptr_Q->Left = T;
	
         ptr_Q->colour = T->colour;
         T->colour = 1;
	
         return ptr_Q;             /*Retorna a nova Raiz*/
      }
   }

   return T;                       /*Retorna o mesmo arvore de entrada sem troca nenhuma*/
}

/*Rotacao a Direita da raiz de T, segundo Rubro Negra pois tambem troca de cor
Esta funcao 'e usada na insercao RubroNegra*/
RowCSR_t *RotacioneDir(RowCSR_t *T){
   RowCSR_t *ptr_Q;

   if(T->Left){ /*Arvore esquerdo nao nulo*/
      if(T->Left->Left && T->Left->Right){ /*Arvore esquerdo nao 'e folha*/
         ptr_Q = T->Left;
         T->Left = ptr_Q->Right;
         ptr_Q->Right = T;
	
         ptr_Q->colour = T->colour;
         T->colour = 1;

      return ptr_Q;               /*Retorna a nova Raiz*/
      }
   }
	return T;                      /*Retorna o mesmo arvore de entrada sem troca nenhuma*/
}

/*  Troca o cor do no raiz e se nao 'e folha tambem troca de cor seus dois filhos.
Esta funcao 'e usada na insercao RubroNegra*/
RowCSR_t *TroqueCores(RowCSR_t *T){

   if(T){ /*Arvore nao vazio*/
      T->colour = (T->colour + 1)%2;  /*cor = 0 ->black, cor = 1 ->red*/
      if(T->Left && T->Right){ /*Nao 'e folha */
         T->Left->colour = (T->Left->colour + 1)%2;
         T->Right->colour = (T->Right->colour + 1)%2;
      }
   }

	return T;
}

/* busca o n� com indice j e retorna o seu valor */
double buscaRN(RowCSR_t *T,unsigned int j){
   if(!T->Left && !T->Right){ /*'e folha */
      return 0.; 
   } 

   if(j < T->column){     /*deve ser inseriado na esquerda*/
      return buscaRN(T->Left,j);
   } else if(j > T->column){                 /*insere na direita*/
      return buscaRN(T->Right,j);
      } else return T->value;
}

/*Insercao de novos n'os para arvores RubroNegras 2-3.  cria um n� com indice j e valor aij*/
RowCSR_t *insere_Rec(RowCSR_t *T,unsigned int j,double aij){

      if(!T->Left && !T->Right){ /*'e folha */
         T = new_node_tree(T,j,aij); 
         return T; 
      } 

      if(j < T->column){     /*deve ser inseriado na esquerda*/
         T->Left = insere_Rec(T->Left,j,aij);
      } else if(j > T->column){                 /*insere na direita*/
         T->Right = insere_Rec(T->Right,j,aij);
        } else T->value  = aij;

		/*Passos para manter o arvore Rubro Negro 2-3 depois de inserir o novo n'o.*/
      if(T->Right->colour && !T->Left->colour){
         T = RotacioneEsq(T);
      }

      if(T->Left->colour && T->Left->Left->colour){
         T = RotacioneDir(T);
      }

      if(T->Left->colour && T->Right->colour){
         T = TroqueCores(T);
      }

return T;
}


RowCSR_t *insere_RN(RowCSR_t* T, unsigned int j, double aij){
  if(j<0) {
      printf("ERRO:index not valid\n");
      exit(1);
  }

/*   if(fabs(aij)>=TOL)   //comentado s'o at'e implementar o suprime em RN*/
      if(T){
         T = insere_Rec(T,j,aij);
         T->colour = 0;
      }
	return T;
}

/*Procura o n� com indice j e ao valor que ele tem soma aij; se o n� n�o existir ent�o o insere.  Ainda n�o est� implementado o suprime Rubro Negra, mais quando estiver pronto se a soma do novo elemento com o elemento armazenado com o n� � zero ent�o esse n� vai ser eliminado*/
RowCSR_t *CSRadd_Rec(RowCSR_t *T,unsigned int j,double aij){

      if(!T->Left && !T->Right){ /*'e folha */
         T = new_node_tree(T,j,aij); 
         return T; 
      } 

      if(j < T->column){     /*deve ser inseriado na esquerda*/
         T->Left = CSRadd_Rec(T->Left,j,aij);
      } else if(j > T->column){                 /*insere na direita*/
         T->Right = CSRadd_Rec(T->Right,j,aij);
        } else{ 
            T->value  += aij;
            /*if(fabs(T->value)<=TOL) suprime(); */
          }

      if(T->Right->colour && !T->Left->colour){
         T = RotacioneEsq(T);
      }

      if(T->Left->colour && T->Left->Left->colour){
         T = RotacioneDir(T);
      }

      if(T->Left->colour && T->Right->colour){
         T = TroqueCores(T);
      }
return T;
}

RowCSR_t *CSRadd(RowCSR_t *T,unsigned int j,double aij){
  if(j<0) {
      printf("ERRO:index not valid\n");
      exit(1);
  }

   if(fabs(aij)>=TOL){
      if(T){
         T = CSRadd_Rec(T,j,aij);
         T->colour = 0;
      }
   }
	return T;
}

/* Produto ponto entre a data armazenada no �rvore de linha T com o vetor x.  Este produto � mantendo as opera��es esparsas.  O n�mero de opera��es depende da quantidade de elementos n�o nulos de T*/
double dot_produc_tree(RowCSR_t *T,double* x){
   if(T->Left || T->Right){
      return  dot_produc_tree(T->Left,x) + x[T->column]*T->value + dot_produc_tree(T->Right,x);
   }

   return 0.0;
}


/**************************************************/
/*Inicio da estrutura para Matriz esparsa */
/* Formato de armazenamento CSR */
/*************************************************/


/*Cria uma matriz esparsa de ordem nxm. Cada fila aponta para um �rvore para ser armazenado por elemento a columna e  o seu valor. Formato CSR*/
MatCSR_t *new_matrix(unsigned int n, unsigned int m){
unsigned int i;
MatCSR_t *M;

   if(!n || !m){
      printf("ERRO:new_matrix: Dimention not valid\n");
      exit(1);
   }

   M =  (MatCSR_t *) malloc (sizeof(MatCSR_t ));
   if(!M) problem();
   M->n = n;
   M->m =m;
   M->Rows = (RowCSR_t **) malloc (n * sizeof(RowCSR_t*));
   if(!M->Rows) problem();

   for(i=0;i<n;i++) M->Rows[i] = new_tree();

   return M;
}

/*Insere o elemento aij na fila i e coluna j da matriz*/
MatCSR_t *set_aij(MatCSR_t* M, unsigned int i, unsigned int j, double aij){
   if(i<0 || j<0 || i>= M->n ||j >= M->m){
      printf("ERRO::set_aij:Index not valid\n");
      exit(1);
   }
   M->Rows[i] = insere_RN(M->Rows[i], j , aij);

   return M;
}

/*Devolve o elemento na fila i e coluna j. Se o elemento n�o estiver armazenado ent�o retorna zero (por ser matriz esparsa)*/
double get_aij(MatCSR_t* M, unsigned int i, unsigned int j){
   if(i<0 || j<0 || i>= M->n ||j >= M->m){
      printf("ERRO::set_aij:Index not valid\n");
      exit(1);
   }

   return buscaRN(M->Rows[i], j);
}

/*Ao elemento na fila i coluna j soma aij.  Esta n�o � n�o soma de matrizes, � uma soma pontoal*/
MatCSR_t *add_aij(MatCSR_t* M, unsigned int i, unsigned int j, double aij){
   if(i<0 || j<0 || i>= M->n ||j >= M->m){
      printf("ERRO::set_aij:Index not valid\n");
      exit(1);
   }

   M->Rows[i] =  CSRadd(M->Rows[i], j , aij);

   return M;
}
               
/*Imprime a matriz em formato CSR, imprimindo por linha cada elemento dado com coluna e valor de dato.
Exemplo a linha [ 0 -2 0 0 5 0 3] � impressa (1,-2) (4,5) (6,3)*/
void imprime_matrix(MatCSR_t *M){
unsigned int i, n;

   n = M->n;
   for(i=0;i<n;i++){
      imprime_Inordem(M->Rows[i]);
      printf("\n");
   }
}

void write_MATLABmatrix(MatCSR_t *M, FILE *output){
unsigned int i, n;

   n = M->n;
   fprintf(output,"A=sparse(%d,%d);\n",n,M->m);
   for(i=0;i<n;i++){
      write_MATLAB(M->Rows[i],i, output); 
   }

}

int get_NNZ(MatCSR_t* M){
unsigned int i, n, nnz = 0;

   n = M->n;
   for(i=0;i<n;i++){
      nnz += contenos(M->Rows[i]);
   }
   return nnz;
}

/*Produto entre a matriz esparsa M com o vetor x e a solu��o e dada em y. y=Ax.
Este produto � feito aproveitando a esparcidad da matriz e o n�mero de opera��es depende do n�mero de elementos diferentes de zero da matriz*/
void mat_vec(MatCSR_t *M,double* x, double* y){
unsigned int i, n;

   n = M->n;
   for(i=0;i<n;i++){
      y[i] = dot_produc_tree(M->Rows[i], x);
   }

}

/*Produto ponto entre dos vetores x1 e x2 com dimens�o dim*/
double ddot(unsigned int dim,double * x1,double* x2){
double temp=0.0;
unsigned int i;

   for(i=0;i<dim;i++)   temp = temp+x1[i]*x2[i];

   return temp;
}

/*y = ax+y . Opera��o entre vetores. faz o produto do escalar a com o vetor x e a soma com o vetor y componente a componente; o resultado � retornado em y*/
void daxpy(double a,unsigned int dim,double* x, double* y){
unsigned int i;

   for(i=0;i<dim;i++)   y[i]=a*x[i]+y[i];

}

/*faz uma copia do vetor x no vetor xCopy*/
void dcopy(unsigned int dim, double* xCopy, double* x){
unsigned int i;

   for(i=0;i<dim;i++)  xCopy[i] = x[i];

}

/*Solver gradiente conjugado, mantendo as opera��es de matriz esparsa e de vetores.  retorna flag que � zero se o sistema n�o converge e um se convergeu com a tolerancia pedida.  Soluciona o sistema  MX = b com condi��o inicial x_0, a saida � o vetor X*/
unsigned int CSR_CG(MatCSR_t *M, double *b, double *x_0, unsigned int max_iter, double tol, double *X){

unsigned int i, dim, flag;
unsigned int num_iter;
double* r;
double* dk;
double* Adk;
double* sol;
double res;
double  ak = 0;
double ddotr = 0;

   flag = 0; /* not converge */ 
   dim = M->n;
   r =  (double *) malloc (dim*sizeof(double));
   dk = (double *) malloc (dim*sizeof(double));
   Adk = (double *) malloc (dim*sizeof(double));
   sol = (double *) malloc (dim*sizeof(double));

   dcopy(dim,X,x_0);
   dcopy(dim, r, b);

   mat_vec(M,X,Adk);
   /*r=b-Ax */
   daxpy(-1.0,dim,Adk,r); 

   /*d0 = r0 */
   dcopy(dim, dk, r);

   /*sol=X */
   dcopy(dim, sol,X);

   res = ddotr = ddot(dim,r,r);

   for(i=0;i<max_iter;i++){
   
      /*ak = <rk,rk>/<Adk,dk> */
      mat_vec(M,dk,Adk);
      ak = ddotr/ddot(dim,Adk, dk);
         
      /*xk+1 = xk + ak(dk) */
      daxpy( ak,dim, dk, sol);
         
      /*r=rk+1 = rk - ak(Adk) */ 
      daxpy( -ak,dim, Adk, r);
   
      /*res = sqrt(<b-A*xk,b-A*xk>) o res=||b-A*xk||=||A*xk-b|| */
      mat_vec(M,sol,Adk);
      daxpy(-1,dim,b,Adk);
      res=ddot(dim,Adk,Adk);
   
      /* dk+1=rk+1+(<rk+1,rk+1>/<rk,rk>)*dk */
      dcopy(dim,Adk,r);
      daxpy(ddot(dim,r,r)/ddotr,dim, dk,Adk);
      dcopy(dim, dk, Adk);
         
      num_iter = num_iter+1;
      ddotr = ddot(dim,r,r);   
            
   
      if (sqrt(res)<=tol){
         flag = 1;
         break;
      }
   }
   
   dcopy(dim, X, sol);
   printf("CGiter = %d com residuo = %1.10f para tol=%1.10f\n",i,sqrt(res),tol);
return flag;
}

