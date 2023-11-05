#include "BoundaryCondition.h"

/**************************************************/
/*Inicio da estrutura para condicoes de fonteira  */
/* fn é um arbol de funcoes onde ficam armazenadas*/ 
/*para cada numeo de fronteira a funcao assinada  */
/*permitindo busca logaritmica                    */
/*set_DN é um vetor onde cada posicao indica se o */
/*numero de condicao de fonteira correspondente é */
/*Dirichlet ou Newman                             */
/*************************************************/

/*Cria uma condicao de fronteira com NC particoes da fronteira, estas vao ser nomeadas desde 0 ate NC-1*/
BoundaryCondition_t *new_BoundaryCondition(unsigned int NC){
   BoundaryCondition_t* BC;
   int i;

   if(!NC){
      printf("ERRO::new_BoundaryCondition:Quantity of conditions not valid\n");
      exit(1);
   }

   BC = (BoundaryCondition_t*) malloc (sizeof(BoundaryCondition_t ));
   if(!BC) problem();

   BC->fn = new_treefn();
   BC->NC = NC;

   BC->set_DN = (int*) malloc (sizeof(int ));
   if(!BC->set_DN) problem();
   for(i=0;i<NC;i++) BC->set_DN[i] = 0;

return BC;
}

void libera_BoundaryCondition(BoundaryCondition_t *BC){

}

/*retorna o tipo de condicao de fronteira na fonteira identificada com o inidice NFC. Dirichlet (1) e Newman (-1), 0 nao inicializado */
int get_BoundaryConditionDN(BoundaryCondition_t *BC,unsigned int NFC){
if(NFC > BC->NC){
   printf("ERRO::get_BoundaryConditionDN:Number of conditions not valid\n");
   exit(1);
}
return BC->set_DN[NFC]; 
}

/*evalua o ponto x,y na funcao de fronteira correspondente ao indice NFC */
double eval_Boundaryfunc(BoundaryCondition_t *BC,unsigned int NFC, double x, double y){
   if(NFC > BC->NC){
      printf("ERRO::eval_Boundaryfunc:Quantity of conditions not valid\n");
      exit(1);
   }
   if(!BC->set_DN[NFC]){
      printf("ERRO::eval_Boundaryfunc:Number of function not assigned\n");
      exit(1);
   }
return evaltreefn(BC->fn,NFC,x,y);
}

/*Armazena a funcao fn corespondente ao indice NFC e que tem condicao de fronteira typeDN na estrutura*/
BoundaryCondition_t *set_Condition_func(BoundaryCondition_t *BC,unsigned int NFC, function_t *fn, int typeDN){
/*   function_t *F;*/

   if(typeDN != -1 && typeDN != 1) {
      printf("ERRO::set_Condition_func:Type of condition not valid, Dirichlet = 1 e Newman =-1\n");
      exit(1);
   }
   if(NFC >= BC->NC){
      printf("ERRO::set_Condition_func:Number of conditions not valid\n");
      exit(1);
   }

/*   F = buscatreefn(BC->tree_fn,NFC);*/
   if(!BC->set_DN[NFC]){
      BC->fn = insere_treefn(BC->fn, NFC,fn);
      BC->set_DN[NFC] = typeDN;
   } else {
         printf("index %d before asigned to ",NFC);
         if(BC->set_DN[NFC] == -1)   printf("Newman condition\n");
         else if(BC->set_DN[NFC] == 1)    printf("Dirichlet condition\n");
       }

   return BC;
}


