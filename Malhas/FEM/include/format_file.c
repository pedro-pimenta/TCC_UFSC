#include "format_file.h"

/*Cria o arquivo de malha no formato compativel*/

Elemno_t *new_Elemno(unsigned int N1, unsigned int N2, unsigned int N3){ 
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
}

list_t * new_ElementList(){
   list_t * L = (list_t *) malloc (sizeof(list_t));
   if(!L) problem();

   L->num = 0;                    /*Quantidde de elementos que tem a lista atualmente */
   L->begin = new_Elemno(0,0,0);  /*cabeca, iniciada com 0 0 0*/
   L->end = L->begin;             /*end*/
   L->begin->prox = NULL;         /*begin*/
   return L;
}

list_t *insertElemList(list_t * L, unsigned int N1,unsigned int N2,unsigned int N3){
Elemno_t * Elemno = new_Elemno(N1,N2,N3);

L->end->prox = Elemno;
L->end = Elemno;
L->num ++;  /* Aumenta a quantidade de elementos da list*/
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
      printf("%d\t%d\t%d\n",N1,N2,N3);
      fprintf(output,"%d\t%d\t%d\n",N1,N2,N3);
   }
}

void libera_List(list_t * L){
   Elemno_t *temp;
   
   while(L->begin != L->end){
      temp = L->begin->prox;
      if(!temp->prox)  L->end = L->begin;
      L->begin->prox = temp->prox;
      L->num--;   /* Diminui a quantidade de elementos da list*/
      libera_Elemno(temp);
   } 
}



/****************************************************/
/*     */
/****************************************************/

BEno_t *new_BEno(unsigned int N1, unsigned int N2, unsigned int NE, int BC){ 
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
}


BElist_t * new_BoundaryEdgeList(){
   BElist_t * L = (BElist_t *) malloc (sizeof(BElist_t));
   if(!L) problem();

   L->num = 0;                    /*Quantidde de BoundaryEdges que tem a lista atualmente */
   L->numBC = 0;                  /*Quantidde de BC diferentes que tem a lista atualmente */
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
L->num ++;  /* Aumenta a quantidade de BoundaryEdges da fila*/
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
      printf("%d\t%d\t%d\t%d\n",N1,N2,NE,BC);
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


/****************************************************/
/*  */
/****************************************************/

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
   for(i=0;i<NumP;i++){
      fscanf(input, "%s",&trash);               /* node-number  */
      fscanf(input, "%lf",&x);
      fscanf(input, "%lf",&y);
      fscanf(input, "%s",&trash);               /* z-coord      */
      fprintf(output,"%f\t%f\n",x,y);
      printf("%f\t%f\n",x,y);
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
        } else {
             printf("ERRO::read_meshGMSH:Type gmsh's element not implemented\n");
             exit(1);
          }
   }

   fprintf(output,"!Triangles\n");
   NumE = TL->num;
   fprintf(output,"%d\n",NumE);
   imprimeListFile(output, TL);

   BEL = set_BEinElement(BEL,TL);
   fprintf(output,"!Edges\n");
   NumE = BEL->num;
   BC = BEL->numBC;
   fprintf(output,"%d\t%d\n",NumE,BC);
   imprimeBEListFile(output,BEL);

   libera_List(TL);
   libera_BEList(BEL);
}

