#include "treefunction.h"

/*******************************************************/
/*Estrutura que controla a inicializacao de funcoes por*/ 
/*meio de aquivos externos                             */
/*******************************************************/

function_t *asigned_function(double (*fn)(double,double)){
  function_t *F;

   F = (function_t *) malloc (sizeof(function_t));
   if(!F) problem();
   F->fn = fn;

  return F;
}


treefn_t *new_treefn(){
  treefn_t *F;

   F = (treefn_t *) malloc (sizeof(treefn_t));
   if(!F) problem();
   F->Left = NULL;
   F->Right = NULL;
   F->colour = 0;
      
  return F;
}

treefn_t *new_node_treefn(treefn_t *F,unsigned int index_fn, function_t *fn){

   if(index_fn>=0){
      F->index = index_fn;
      F->fn = fn;
      F->colour = 1;
      F->Left = new_treefn();
      F->Right = new_treefn();
   }
      
  return F;
}

void imprime_Inordem_indextreefn(treefn_t *F){
  if(F->Left || F->Right){
      imprime_Inordem_indextreefn(F->Left);
      printf(" %d ",F->index);
      imprime_Inordem_indextreefn(F->Right);
   }
}

treefn_t *RotacioneEsqtreefn(treefn_t *F){
   treefn_t *ptr_Q;

   if(F->Right){ /*Arvore direito nao nulo*/
      if(F->Right->Left && F->Right->Right){ /*Arvore esquerdo nao 'e folha*/
         ptr_Q = F->Right;
         F->Right = ptr_Q->Left;
         ptr_Q->Left = F;
	
         ptr_Q->colour = F->colour;
         F->colour = 1;
	
         return ptr_Q;             /*Retorna a nova Raiz*/
      }
   }

   return F;                       /*Retorna o mesmo arvore de entrada sem troca nenhuma*/
}


treefn_t *RotacioneDirtreefn(treefn_t *F){
   treefn_t *ptr_Q;

   if(F->Left){ /*Arvore esquerdo nao nulo*/
      if(F->Left->Left && F->Left->Right){ /*Arvore esquerdo nao 'e folha*/
         ptr_Q = F->Left;
         F->Left = ptr_Q->Right;
         ptr_Q->Right = F;
	
         ptr_Q->colour = F->colour;
         F->colour = 1;

      return ptr_Q;               /*Retorna a nova Raiz*/
      }
   }
	return F;                      /*Retorna o mesmo arvore de entrada sem troca nenhuma*/
}

treefn_t *TroqueCorestreefn(treefn_t *F){

   if(F){ /*Arvore nao vazio*/
      F->colour = (F->colour + 1)%2;  /*cor = 0 ->black, cor = 1 ->red*/
      if(F->Left && F->Right){ /*Nao 'e folha */
         F->Left->colour = (F->Left->colour + 1)%2;
         F->Right->colour = (F->Right->colour + 1)%2;
      }
   }

	return F;
}

function_t *buscatreefn(treefn_t *F,unsigned int j){
   if(!F->Left && !F->Right){ /*'e folha */
      return NULL; 
   } 

   if(j < F->index){     /*deve ser inseriado na esquerda*/
      return buscatreefn(F->Left,j);
   } else if(j > F->index){                 /*insere na direita*/
      return buscatreefn(F->Right,j);
      } else return F->fn;
}

double evaltreefn(treefn_t *F,unsigned int j, double x, double y){
function_t *fn;
   fn = buscatreefn(F,j);

   if(!F){ /*'e folha */
      printf("function with index %d not asigned, operation not valid\n",j);
   } else {
      return fn->fn(x,y);
   }
   exit(1);
}

treefn_t *insere_Rectreefn(treefn_t *F,unsigned int j,function_t *fn){

      if(!F->Left && !F->Right){ /*'e folha */
         F = new_node_treefn(F,j,fn); 
         return F; 
      } 

      if(j < F->index){     /*deve ser inseriado na esquerda*/
         F->Left = insere_Rectreefn(F->Left,j,fn);
      } else if(j > F->index){                 /*insere na direita*/
         F->Right = insere_Rectreefn(F->Right,j,fn);
        } else /*F->fn  = fn;*/
            printf("function with index %d asigned before, operation not valid\n",j);

		/*Passos para manter o arvore Rubro Negro 2-3 depois de inserir o novo n'o.*/
      if(F->Right->colour && !F->Left->colour){
         F = RotacioneEsqtreefn(F);
      }

      if(F->Left->colour && F->Left->Left->colour){
         F = RotacioneDirtreefn(F);
      }

      if(F->Left->colour && F->Right->colour){
         F = TroqueCorestreefn(F);
      }

return F;
}


treefn_t *insere_treefn(treefn_t* F, unsigned int j, function_t * fn){
  if(j<0) {
      printf("ERRO:index not valid\n");
      exit(1);
  }

/*   if(fabs(fn)>=FOL)   //comentado s'o at'e implementar o suprime em RN*/
      if(F){
         F = insere_Rectreefn(F,j,fn);
         F->colour = 0;
      }
	return F;
}
