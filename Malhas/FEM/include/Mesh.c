#include "Mesh.h"

/**************************************************/
/*Inicio da estrutura para o controle da Malha */
/* Esta estrutura usa points, Elements e Boundary Edge */
/*************************************************/


/* Nova malha.  
O formato para a estrutura de dados est� dado por:
1. NP : Quantidade total de pontos.  
2. NE : Quantidade total de triangulos. 
3. NBE : Quantidade de arestas.
Com toda a informa��o acima � criado o alocamante para os ponteros de estrutura correspondente.  Por exemplo um vetor de pontos de tamanho NP, tambem cria um vetor de elementos de tamanho NE  e assim sucesivamente*/
Mesh_t *new_Mesh(){
   Mesh_t *M;

   M = (Mesh_t *) malloc (sizeof(Mesh_t));
   if(!M) problem();
   M->NP = 0;
   M->NE = 0;
   M->NBE = 0;
   return M;
}

void libera_Mesh(Mesh_t *M){
unsigned int i, Num;
   if(M){
      Num = M->NP;
      if(Num){
         for(i=0;i<Num;i++)   libera_point(M->Points[i]);
      }
      Num = M->NE;
      if(Num){
         for(i=0;i<Num;i++) libera_Element(M->Elements[i]);
      }
      Num = M->NBE;
      if(Num){
         for(i=0;i<Num;i++)   libera_BoundaryEdge(M->BE[i]);
         libera_BoundaryCondition(M->BC);
      }
      free(M);
   }
}

/* Retorna o ponto no indice index da malha*/
point_t* get_Meshpoint(Mesh_t *M, unsigned int index){
point_t* rtn_p, *ptr_p;
double x, y;
   if(M->NP){
      if(index<M->NP){
         ptr_p = M->Points[index];
         if(ptr_p){
            x = ptr_p->x;
            y = ptr_p->y;
            rtn_p = new_point(x,y);
            return rtn_p;
         }

      } else {
            printf("ERRO::get_Meshpoint:index out of range\n");
            exit(1);
        }
   } else {
         printf("ERRO::get_Meshpoint:first used set_MeshNumpoint\n");
         exit(1);
     }
   return NULL;
}

/* Retorna o elemento no indice index da malha*/
Element_t* get_MeshElement(Mesh_t *M, unsigned int index){
Element_t* rtn_T, *ptr_T;
unsigned int N1, N2, N3;
   if(M->NE){
      if(index<M->NE){
         ptr_T = M->Elements[index];
         if(ptr_T){
            N1 = ptr_T->N1;
            N2 = ptr_T->N2;
            N3 = ptr_T->N3;
            rtn_T = new_Element(N1,N2,N3);
            return rtn_T;
         }
      } else {
            printf("ERRO::get_MeshElement:index out of range\n");
            exit(1);
        }
   } else {
         printf("ERRO::set_MeshElement:first used set_MeshNumElement\n");
         exit(1);
     }
   return NULL;
}

/* Retorna a BoundaryEdge no indice index da malha*/
BoundaryEdge_t* get_MeshBE(Mesh_t *M, unsigned int index){
BoundaryEdge_t* rtn_BE, *ptr_BE;
unsigned int N1,N2, NE;
int BC;
   if(M->NBE){
      if(index<M->NBE){
         ptr_BE = M->BE[index];
         if(ptr_BE){
            N1 = ptr_BE->N1;
            N2 = ptr_BE->N2;
            NE = ptr_BE->NE;
            BC = ptr_BE->BC;
            rtn_BE = new_BoundaryEdge(N1,N2,NE,BC);
            return rtn_BE;
         }
      } else {
            printf("ERRO::get_MeshBE:index out of range\n");
            exit(1);
        }
   } else {
         printf("ERRO::get_MeshBE:first used set_MeshNumBE\n");
         exit(1);
     }
   return NULL;
}

/*Inicializa a funcao de fronteira com indice NFC relacionada com a funcao fn e de tipo typeDN*/
Mesh_t *setmesh_BCfunc(Mesh_t *M,unsigned int NFC, function_t *fn, int typeDN){
   M->BC = set_Condition_func(M->BC,NFC,fn,typeDN);
return M;
}

int get_meshBCtype(Mesh_t *M,unsigned int NFC){
   return get_BoundaryConditionDN(M->BC,NFC);
}

double eval_meshBCfunc(Mesh_t *M,unsigned int NFC, double x, double y){
return eval_Boundaryfunc(M->BC,NFC,x,y);
}


/*lee a malha de um arquivo. O formato de entrada �:
1. NP
2. pontos x_0 y_0
          x_1 y_1
          ...
          x_NP-1 y_NP-1
3. NE 
4. indices dos vertices de triangulos, segundo a numeracao de pontos
           N1_0 N2_0 N3_0
           N1_1 N2_1 N3_1
           ...
           N1_NE-1 N2_NE-1 N3_NE-1
5. NBE 
9. aristas e condi��o de fronteira, os indices s�o segundo a numera��o de pontos
           N1_0 N2_0 NE_0 BC_0
           N1_1 N2_1 NE_1 BC_1
           ...
           N1_NBE-1 N2_NBE-1 NE_NBE-1 BC_NBE-1
*/
Mesh_t* read_Mesh(FILE *input){
int Num, N1, N2, N3, NE;
double x,y;
unsigned int i;
int BC;
Mesh_t* M;
char trash;
double xx, yy, dist;
double Area;



   M = new_Mesh();
   M->GreaterArea = 0.;
   M->GreaterE = 0.;

   fscanf(input, "%s",&trash);      /* !points */

   /*we start reading points*/
   fscanf(input, "%d",&Num);
   if(Num < 0){
      printf("ERRO::read_mesh:Number of points is not valid\n");
      exit(1);
   }
   M->NP =  (unsigned int) Num;
   M->Points = (point_t **) malloc (Num*sizeof(point_t*)); 
   if(!M->Points) problem();
   for(i=0;i<Num;i++){
      fscanf(input, "%lf",&x);
      fscanf(input, "%lf",&y);
      M->Points[i] = new_point(x,y);
   }
  
   fscanf(input, "%s",&trash);      /* !triangles */

   /*we start reading Elements*/
   fscanf(input, "%d",&Num);
   if(Num < 0){
      printf("ERRO::read_mesh:Number of Elements is not valid\n");
      exit(1);
   }
   M->NE =  (unsigned int) Num;
   M->Elements = (Element_t **) malloc (Num*sizeof(Element_t*));
   if(!M->Elements) problem();
   for(i=0;i<Num;i++){
      fscanf(input, "%d",&N1);
      fscanf(input, "%d",&N2);
      fscanf(input, "%d",&N3);
      if(N1<0 || N2<0 || N3<0){
         printf("ERRO::read_mesh:Some index of node for element is not valid\n ");
         exit(1);
      }
      M->Elements[i] = new_Element((unsigned int) N1,(unsigned int) N2,(unsigned int) N3);
      
      xx = (M->Points[N1]->x - M->Points[N2]->x) * (M->Points[N1]->x - M->Points[N2]->x);
      yy = (M->Points[N1]->y - M->Points[N2]->y) * (M->Points[N1]->y - M->Points[N2]->y);
      dist = xx + yy; 

      Area = fabs( (M->Points[N2]->x*M->Points[N3]->y - M->Points[N3]->x*M->Points[N2]->y) - M->Points[N1]->x*(M->Points[N3]->y - M->Points[N2]->y) + M->Points[N1]->y*(M->Points[N3]->x - M->Points[N2]->x) );
   
      if(dist > M->GreaterE)  M->GreaterE = dist;
      if(Area > M->GreaterArea)   M->GreaterArea = Area;

      xx = (M->Points[N1]->x - M->Points[N3]->x) * (M->Points[N1]->x - M->Points[N3]->x);
      yy = (M->Points[N1]->y - M->Points[N3]->y) * (M->Points[N1]->y - M->Points[N3]->y);
      dist = xx + yy; 
      if(dist > M->GreaterE)  M->GreaterE = dist;

      xx = (M->Points[N3]->x - M->Points[N2]->x) * (M->Points[N3]->x - M->Points[N2]->x);
      yy = (M->Points[N3]->y - M->Points[N2]->y) * (M->Points[N3]->y - M->Points[N2]->y);
      dist = xx + yy; 
      if(dist > M->GreaterE)  M->GreaterE = dist;

   }

   M->GreaterE = sqrt(M->GreaterE);
   M->GreaterArea = M->GreaterArea*0.5;
   printf("A maior arista tem dist = %f e a maior area 'e:%.12f\n",M->GreaterE,M->GreaterArea);

   fscanf(input, "%s",&trash);      /* !Edges */

   /*we start reading BoundaryEdges*/
   fscanf(input, "%d",&Num);
   if(Num <= 0){
      printf("ERRO::read_mesh:Number of BoundaryEdges not valid\n");
      exit(1);
   }
   M->NBE =  (unsigned int) Num;
   M->BE = (BoundaryEdge_t **) malloc (Num*sizeof(BoundaryEdge_t*));
   if(!M->BE) problem();
   fscanf(input, "%d",&Num);
   if(Num <= 0){
      printf("ERRO::read_mesh:Quantity of conditions is not valid\n");
      exit(1);
   }
   M->BC = new_BoundaryCondition((unsigned int) Num);
   for(i=0;i<M->NBE;i++){
      fscanf(input, "%d",&N1);
      fscanf(input, "%d",&N2);
      fscanf(input, "%d",&NE);
      fscanf(input, "%d",&BC);
      if(N1<0 || N2<0 || NE<0){
         printf("ERRO::read_mesh:Some index of node for BoundaryEdge is not valid\n ");
         exit(1);
      }
      if(BC >= Num || BC<0){
         printf("ERRO::read_mesh:Number condiction %d to function is not valid\n ",BC);
         exit(1);
      }
      M->BE[i] = new_BoundaryEdge((unsigned int) N1,(unsigned int) N2,(unsigned int) NE,BC);
   }

return M;
}


/* Cria um arquivo saida.mesh com a informa��o da malha*/
void write_Mesh(Mesh_t *M, FILE *output){
unsigned int Num;
double x,y;
unsigned int i, N1, N2, N3, NE;
int BC;

   fprintf(output,"!Points\n");
   Num = M->NP;
   if(Num <= 0){
      printf("ERRO::write_mesh:Number of points not asigned\n");
      exit(1);
   }
   fprintf(output,"%d\n",Num);

   for(i=0;i<Num;i++){
      x = M->Points[i]->x;
      y = M->Points[i]->y;
      fprintf(output, "%f\t%f\n",x,y);
   }
  
   fprintf(output,"!Triangles\n");
   Num = M->NE;
   if(Num <= 0){
      printf("ERRO::write_mesh:Number of Elements not asigned\n");
      exit(1);
   }
   fprintf(output, "%d\n",Num);

   for(i=0;i<Num;i++){
      N1 = M->Elements[i]->N1;
      N2 = M->Elements[i]->N2;
      N3 = M->Elements[i]->N3;
      fprintf(output, "%d\t%d\t%d\n",N1,N2,N3);
   }

   fprintf(output,"!Edges\n");
   Num = M->NBE;
   if(Num <= 0){
      printf("ERRO::write_mesh:Number of BoundaryEdges not asigned\n");
      exit(1);
   }
   fprintf(output, "%d\t",Num);
   Num = M->BC->NC;
   fprintf(output, "%d\n",Num);

   for(i=0;i<Num;i++){
      N1 = M->BE[i]->N1;
      N2 = M->BE[i]->N2;
      NE = M->BE[i]->NE;
      BC = M->BE[i]->BC;
      fprintf(output, "%d\t%d\t%d\t%d\n",N1,N2,NE,BC);
   }
}
