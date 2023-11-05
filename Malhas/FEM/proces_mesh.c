
/* modo de uso: */
/* gcc -Wall -pedantic -O2 -lm proces_mesh.c -o exe  */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void problem(){
   printf("Erro: Insufficient memory!!!\n");
   exit(1);
}



/**************************************************/
/*begin da estrutura para as condi��es de fronteira*/
/* Ainda n�o est� sendo muito usada por que por encuanto esta implementado o elementos finitos para o problema em particular, mas para o caso geral vai ser muito util        */
/*************************************************/

typedef struct BoundaryEdge{
   unsigned int N1;
   unsigned int N2;
   unsigned int NE;  /*Number of element*/
   int BC;
   
}BoundaryEdge_t;

/*Cria um apontador de BoundaryEdge, com os indices N1, N2 que ligam a uma lista de pontos e a respetiva condi��o de fronteira.
BC = -1 � condi��o de Dirichlet, BC = 0 � condi��o de Newman.
A arista � dada pelo segmento de reta p0,p1 e N1 liga com p0 e N2 liga com p1.
� recomendado que a orienta��o dos vertices sejam num sentido s�, horario o antihorario.*/
BoundaryEdge_t *new_BoundaryEdge(unsigned int N1,unsigned int N2,unsigned int NE, int BC){
   BoundaryEdge_t *BE;

   BE = (BoundaryEdge_t *) malloc (sizeof(BoundaryEdge_t));
   if(!BE) problem();
   BE->N1 = N1;
   BE->N2 = N2;
   BE->NE = NE;
   BE->BC = BC;

   return BE;
}

void libera_BoundaryEdge(BoundaryEdge_t *BE){
   if(BE){
      free(BE);
   }
}

/* Retorna o indice 1 de BE*/
unsigned int get_BoundaryEdgeN1(BoundaryEdge_t *BE){
   return BE->N1;
}

/* Retorna o indice 2 de BE*/
unsigned int get_BoundaryEdgeN2(BoundaryEdge_t *BE){
   return BE->N2;
}

/* Retorna o indice do elemento ao qual pertence*/
unsigned int get_BoundaryNumElement(BoundaryEdge_t *BE){
   return BE->NE;
}

/* Retorna a condi��o de fronteira de BE*/
int get_BoundaryEdgeBC(BoundaryEdge_t *BE){
   return BE->BC;
}

/* Imprime os dados de BE*/
void print_BoundaryEdge(BoundaryEdge_t* BE){
   printf(" %d %d %d %d\n",BE->N1, BE->N2, BE->NE, BE->BC);
}

/**************************************************/
/*begin da estrutura para Elementos triangulares */
/*************************************************/

typedef struct Element{
   unsigned int N1;
   unsigned int N2;
   unsigned int N3;
}Element_t;

/* Elemnovo Elemento de triangulo com vertices p0, p1 e p2 que s�o ligados atraves dos indices N1, N2 e N3.  */
Element_t *new_Element(unsigned int N1,unsigned int N2, unsigned int N3){
   Element_t *T;

   T = (Element_t *) malloc (sizeof(Element_t));
   if(!T) problem();
   T->N1 = N1;
   T->N2 = N2;
   T->N3 = N3;

   return T;
}

void libera_Element(Element_t *T){
   if(T){
      free(T);
   }
}

/* Retorna o indice 1 de T*/
unsigned int get_ElementN1(Element_t *T){
   return T->N1;
}

/* Retorna o indice 2 de T*/
unsigned int get_ElementN2(Element_t *T){
   return T->N2;
}

/* Retorna o indice 3 de T*/
unsigned int get_ElementN3(Element_t *T){
   return T->N3;
}




typedef struct Elemno{
   Element_t * T;         /* Valor inteiro para armazenar cada digito binario, em 
   momentos � inicializado com -1 para prevenir possiveis erros pois isto
   se tal valor ficase vou opter erros na execu��o */
   struct Elemno * prox;
}Elemno_t;


typedef struct ElementList{
   int num;         /*quantidade de elementos que tem atualmente a fila*/
   Elemno_t * begin;
   Elemno_t * end;
} list_t;


Elemno_t *new_Elemno(unsigned int N1, unsigned int N2, unsigned int N3){ /*Para a fila*/
   Elemno_t *n;
   n = (Elemno_t *) malloc (sizeof(Elemno_t));
   if(!n) problem();
   n->T = new_Element(N1,N2,N3);
   n->prox = NULL;
   return n;
}

void libera_Elemno(Elemno_t *n){
   if(n){
      libera_Element(n->T);
      free(n);
   }
/*  printf("Elemno liberado\n");*/
}


list_t * new_ElementList(){
   list_t * L = (list_t *) malloc (sizeof(list_t));
   if(!L) problem();

   L->num = 0;                    /*Quantidde de elementos que tem a fila atualmente */
   L->begin = new_Elemno(0,0,0);      /*cabe�a, iniciada com -1 para detetar erros*/
   L->end = L->begin;             /*end*/
   L->begin->prox = NULL;         /*begin*/
   return L;
}

list_t *insertElemList(list_t * L, unsigned int N1,unsigned int N2,unsigned int N3){
Elemno_t * Elemno = new_Elemno(N1,N2,N3);

L->end->prox = Elemno;
L->end = Elemno;
L->num ++;  /* Aumenta a quantidade de elementos da fila*/
return L;
}

void imprimeListFile(FILE *output, list_t * f){
   Elemno_t *seg;
   int N1,N2,N3;

   seg = f->begin;
   while(seg->prox){
      seg = seg->prox;
      N1 = seg->T->N1;
      N2 = seg->T->N2;
      N3 = seg->T->N3;
      if(N1 < 0 || N2 < 0 || N3 <0){
         printf("ERRO::imprimeListFile:new numeration for vertex Elemnot valid\n");
         exit(1);
      }
      /*printf("%d\t%d\t%d\n",N1,N2,N3);*/
      fprintf(output,"%d\t%d\t%d\n",N1,N2,N3);
   }
}

void libera_List(list_t * L){
   Elemno_t *temp;
   
   while(L->begin != L->end){
      temp = L->begin->prox;
      if(!temp->prox)  L->end = L->begin;
      L->begin->prox = temp->prox;
      L->num--;   /* Diminui a quantidade de elementos da fila*/
      libera_Elemno(temp);
   } 
}



typedef struct BEno{
   BoundaryEdge_t * BE;         /* Valor inteiro para armazenar cada digito binario, em 
   momentos � inicializado com -1 para prevenir possiveis erros pois isto
   se tal valor ficase vou opter erros na execu��o */
   struct BEno * prox;
}BEno_t;


typedef struct BoundaryEdgeList{
   int num;         /*quantidade de BoundaryEdgeos que tem atualmente a fila*/
   int numBC;
   BEno_t * begin;
   BEno_t * end;
} BElist_t;


BEno_t *new_BEno(unsigned int N1, unsigned int N2, unsigned int NE, int BC){ /*Para a fila*/
   BEno_t *n;
   n = (BEno_t *) malloc (sizeof(BEno_t));
   if(!n) problem();
   n->BE = new_BoundaryEdge(N1,N2,NE,BC);  
   n->prox = NULL;
   return n;
}

void libera_BEno(BEno_t *n){
   if(n){
      libera_BoundaryEdge(n->BE);
      free(n);
   }
/*  printf("BEno liberado\n");*/
}


BElist_t * new_BoundaryEdgeList(){
   BElist_t * L = (BElist_t *) malloc (sizeof(BElist_t));
   if(!L) problem();

   L->num = 0;                    /*Quantidde de BoundaryEdges que tem a fila atualmente */
   L->numBC = 0;                  /*Quantidde de BC diferentes que tem a fila atualmente */
   L->begin = new_BEno(0,0,0,0);  /*cabeca, iniciada com -1 para detetar erros*/
   L->end = L->begin;             /*end*/
   L->begin->prox = NULL;         /*begin*/
   return L;
}

BElist_t *insertBEList(BElist_t * L, unsigned int N1,unsigned int N2,unsigned int NE, int BC){
int find = 1;
BEno_t *seg;
BEno_t * BEno = new_BEno(N1,N2,NE,BC);

seg = L->begin;
while(seg->prox && find){
   seg = seg->prox;
   if (BC == seg->BE->BC)  find = 0;
}

if(find)   L->numBC++;
L->end->prox = BEno;
L->end = BEno;
L->num ++;  /* Aumenta a quantidade de BoundaryEdgeos da fila*/
return L;
}

void imprimeBEListFile(FILE *output,BElist_t *L){
   BEno_t *seg;
   int N1,N2,NE,BC;

   seg = L->begin;
   while(seg->prox){
      seg = seg->prox;
      N1 = seg->BE->N1;
      N2 = seg->BE->N2;
      NE = seg->BE->NE;
      BC = seg->BE->BC;
      if(N1 < 0 || N2 < 0 || NE <0){
         printf("ERRO::imprimeListFile:new numeration for vertex BEnot valid\n");
         exit(1);
      }
      /*printf("%d\t%d\t%d\t%d\n",N1,N2,NE,BC);*/
      fprintf(output,"%d\t%d\t%d\t%d\n",N1,N2,NE,BC);
   }
}

BElist_t *set_BEinElement(BElist_t *BEL,list_t *TL){
   BEno_t *segBE;
   Elemno_t *segT;
   int N1, N2, NP, NE, next;

   NP = 0;
   segBE = BEL->begin;
   while(segBE->prox){
      segBE = segBE->prox;
      N1 = segBE->BE->N1;
      N2 = segBE->BE->N2;

      NE = 0; next = 1;
      segT =TL->begin;
      while(segT->prox && next){
         segT = segT->prox;

         if(N1 == segT->T->N1)   NP++;
         else if(N1 == segT->T->N2)   NP++;
              else if(N1 == segT->T->N3)   NP++;

         if(NP){
            if(N2 == segT->T->N1)   NP++;
            else if(N2 == segT->T->N2)   NP++;
                 else if(N2 == segT->T->N3)   NP++;
            if(NP == 2) next = 0;
            else NE++;
         } else NE++;
         NP = 0;
      }
      if(next){
         printf("ERRO::set_BEinElement:Edge without element\n");
         exit(1);
      }

      segBE->BE->NE = NE;
   }

   return BEL;
}

void libera_BEList(BElist_t * L){
   BEno_t *temp;
   
   while(L->begin != L->end){
      temp = L->begin->prox;
      if(!temp->prox)  L->end = L->begin;
      L->begin->prox = temp->prox;
      L->num--;   /* Diminui a quantidade de BoundaryEdgeos da fila*/
      libera_BEno(temp);
   } 
}





void readGMSH_writeMesh(FILE *input,FILE *output){
   int Num, N1, N2, N3, NE;
   double x,y;
   unsigned int i;
   char trash;
   int BC, NumP, NumE;
   int Etype, Numtag;
   list_t *TL;
   BElist_t *BEL;



   fscanf(input, "%s",&trash);            /* $MeshFormat    */
   fscanf(input, "%s",&trash);            /* version        */
   fscanf(input, "%s",&trash);            /* format file    */
   fscanf(input, "%s",&trash);            /* 8              */
   fscanf(input, "%s",&trash);            /* $EndMeshFormat */
   fscanf(input, "%s",&trash);            /* $Nodes         */

   /*we start reading points*/
   fscanf(input, "%d",&NumP);
   if(NumP < 0){
      printf("ERRO::read_meshGMSH:Number of points is not valid\n");
      exit(1);
   }
   fprintf(output,"!Points\n");
   fprintf(output,"%d\n",NumP);
   printf("Num points = %d\n",NumP);
   for(i=0;i<NumP;i++){
      fscanf(input, "%s",&trash);               /* node-number  */
      fscanf(input, "%lf",&x);
      fscanf(input, "%lf",&y);
      fscanf(input, "%s",&trash);               /* z-coord      */
      fprintf(output,"%f\t%f\n",x,y);
      /*printf("%f\t%f\n",x,y);*/
   }
    
   fscanf(input, "%s",&trash);                  /* $EndNodes    */
   fscanf(input, "%s",&trash);                  /* $Elements    */

   /*we start reading triangles and BoundaryEdges*/
   NE = 0;
   TL = new_ElementList();
   BEL = new_BoundaryEdgeList();
   if(!TL || !BEL) problem();

   fscanf(input, "%d",&NumE);
   if(NumE < 0){
      printf("ERRO::read_meshGMSH:Quantity of gmsh elements is not valid\n");
      exit(1);
   }

   for(i=0;i<NumE;i++){
      fscanf(input, "%s",&trash);               /* elm-number     */
      fscanf(input, "%d",&Etype);
      fscanf(input, "%d",&Numtag);              /* Numtag = 3     */
      if(Etype == 1){                           /* Edge */
         fscanf(input, "%d",&Num);
         fscanf(input, "%d",&BC);
         fscanf(input, "%s",&trash);            /* mesh partition */
         fscanf(input, "%d",&N1);
         fscanf(input, "%d",&N2);
         N1--;   N2--;  BC--;
         if(N1<0 || N2<0 || BC<0 || N1 >= NumP || N2 >= NumP){
            printf("ERRO::read_meshGMSH:Some index of Edge or condition number to boundary is not valid\n ");
            exit(1);
         }
         BEL = insertBEList(BEL,N1,N2,0,BC);
      } else if(Etype == 2){                    /* Element - triangle */
         fscanf(input, "%d",&Num);              /* unused */
         fscanf(input, "%d",&BC);               /* unused */
         fscanf(input, "%s",&trash);            /* mesh partition */
         fscanf(input, "%d",&N1);
         fscanf(input, "%d",&N2);
         fscanf(input, "%d",&N3);
         N1--;   N2--;   N3--; 
         if(N1<0 || N2<0 || N3<0){
            printf("ERRO::read_meshGMSH:Some index of Element is not valid\n ");
            exit(1);
         }
         NE++;
         TL = insertElemList(TL,N1,N2,N3);
        } else if(Etype == 15){                    /* node - not processed */
            fscanf(input, "%s",&trash);     
            fscanf(input, "%s",&trash);     
            fscanf(input, "%s",&trash);     
            fscanf(input, "%s",&trash);     
          } else{
               printf("ERRO::readGMSH_writeMesh:Type %d gmsh's element\n",Etype);
               exit(1);
            }
   }

   fprintf(output,"!Triangles\n");
   NumE = TL->num;
   fprintf(output,"%d\n",NumE);
   printf("Num Triangles Elements = %d\n",NumE);
   imprimeListFile(output, TL);

   BEL = set_BEinElement(BEL,TL);
   fprintf(output,"!Edges\n");
   NumE = BEL->num;
   BC = BEL->numBC;
   fprintf(output,"%d\t%d\n",NumE,BC);
   printf("Num Edges = %d\n",NumE);
   imprimeBEListFile(output,BEL);

   libera_List(TL);
   libera_BEList(BEL);
}


int main(int argc, char **argv){
   FILE* input, *output;
   char *file_input = NULL; /* Nome do arquivo de entrada*/
   char *file_output = NULL;/* Nome do arquivo de saida*/
   char ext[8] = ".Kmesh";
   int i, size;
   

   if(argc >2 || argc <1){
      printf("Modo de uso:\n");
      printf("FEM:\t$ ./executave nome_arquivo.msh\n");
      return(1);
   }
   
   file_input = argv[1];
   file_output = argv[1];
   size = strlen(file_input);
   i = size - 4;

   if(file_input[i] != '.' || file_input[i+1] != 'm' || file_input[i+2] != 's' || file_input[i+3] != 'h') {
      printf("Modo de uso:\n");
      printf("FEM:\t$ ./executave nome_arquivo.msh\n");
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
   output = fopen(file_output,"w");
   if (!output) {
      printf("Erro: write exite file!\n"); 
      exit(1);
   }

   /*Leitura do arquivo da malha *.msh com toda a informacao dada segundo como 'e explicado acima para ser compativel com a estrutura de dados feita*/
   readGMSH_writeMesh(input,output);


return 0;
}
