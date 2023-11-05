#include "triangle.h"

/**************************************************/
/*Inicio da estrutura para Elementos triangulares */
/*************************************************/

/* Novo Elemento de triangulo com vertices p0, p1 e p2 que s�o ligados atraves dos indices N1, N2 e N3.  */
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

