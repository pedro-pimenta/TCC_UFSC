#include "../functions.c"

#include <string.h>
#include "format_file.h"
#include "CSRmat.h"
#include "point.h"
#include "BoundaryEdge.h"
#include "triangle.h"
#include "treefunction.h"
#include "BoundaryCondition.h"
#include "Mesh.h"
#include "FEM.h"



int main(int argc, char **argv){
   Mesh_t* M;
   FEM_t *fem;
   function_t* F1, *F2, *F3, *F4, *Exact, *fxy;
   FILE* input, *output;
   char *file_input = NULL; /* Nome do arquivo de entrada*/
   char *file_output = NULL;/* Nome do arquivo de saida*/
   char ext[8] = "dat";
   int i, size, NNZ;

   if(argc != 2){
      printf("Modo de uso:\n");
      printf("FEM:\t$ ./executave Nome_arquivo.kmesh\n");
      printf("FEM:\t$ ./executave Nome_arquivo.msh\n");
      return(1);
   }

   file_input = argv[1];
   file_output = argv[1];
   size = strlen(file_input);
   i = size - 4;

   if(file_input[i-1] != 'K' || file_input[i] != 'm' || file_input[i+1] != 'e' || file_input[i+2] != 's' || file_input[i+3] != 'h'){
         printf("Modo de uso:\n");
         printf("FEM:\t$ ./executave Nome_arquivo.kmesh\n");
         printf("Transforme o arquivo *.msh no formato compativel com proces_mesh.c");
         return(1);
    }

   file_output = (char*) malloc ((size+8)*sizeof(char ));
   if(!file_output) problem();
   strncat(file_output,file_input,size);
   file_output[i] = '\0';
   strncat(file_output,ext,6);

   input = fopen(file_input,"r");
   if (!input) {
      printf("Erro: read entrance file!\n"); 
      exit(1);
   }


   /*Leitura do arquivo da malha com toda a informacao dada segundo como 'e explicado acima para ser compativel com a estrutura de dados feita*/
   M = read_Mesh(input);

   Exact = asigned_function(U); /*Sol exacta */
   fxy = asigned_function(f);   /* rhs */

   F1 = asigned_function(f1);
   F2 = asigned_function(f2);
   F3 = asigned_function(f3);
   F4 = asigned_function(f4);

   /*condicoes de fronteira do exercicio proposto*/
    M = setmesh_BCfunc(M,0, F1, 1);
    M = setmesh_BCfunc(M,1, F2, -1);
    M = setmesh_BCfunc(M,2, F3, -1);
    M = setmesh_BCfunc(M,3, F4, 1);

   /*condicoes de fronteira Dirichlet para o exercicio proposto */
/*  M = setmesh_BCfunc(M,0, Exact, 1);
    M = setmesh_BCfunc(M,1, Exact, 1);
    M = setmesh_BCfunc(M,2, Exact, 1);
    M = setmesh_BCfunc(M,3, Exact, 1); */

   /*condicoes de fronteira dirichlet para uma fronteira subdividida em 8 sub-fronteiras*/
/*   M = setmesh_BCfunc(M,0, Exact, 1);
   M = setmesh_BCfunc(M,1, Exact, 1);
   M = setmesh_BCfunc(M,2, Exact, 1);
   M = setmesh_BCfunc(M,3, Exact, 1);
   M = setmesh_BCfunc(M,4, Exact, 1);
   M = setmesh_BCfunc(M,5, Exact, 1);
   M = setmesh_BCfunc(M,6, Exact, 1);
   M = setmesh_BCfunc(M,7, Exact, 1); */


   /*Set das variaveis da estrutura FEM, segundo os dados na malha D.  Criacao das bases por elemento*/
   fem = new_FEM(M,fxy);
   /*Criacao da matriz e do vetor para formarem o sistema FEM seguindo o algoritmo 12.5, depois a solucao do sistema 'e feita por gradiente conjugado e finalmente sao impresos na tela alguns dos dados obtidos*/
   fem = set_systemFEM(fem);

   NNZ = get_NNZ(fem->A);

   printf("sistem dimension = %dx%d = %d com %d valores diferentes de zero\n",fem->NPinte_newman,fem->NPinte_newman,fem->NPinte_newman*fem->NPinte_newman,NNZ);
   get_maxerro(fem,Exact);
   get_erroL2(fem,Exact);


/*Para informacao em arquivos ou na tela */
/*
   output = fopen(file_output,"w");
   if (!output) {
      printf("Erro: write exit file!\n"); 
      exit(1);
   }
   write_noderro(fem,Exact,output);
*/

   /* print_info(fem,Exact); */


   libera_Mesh(M);

return 0;
}
