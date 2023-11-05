#ifndef T_
#define T_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "msg.h"

/**************************************************/
/*Inicio da estrutura para Elementos triangulares */
/*************************************************/

typedef struct Element{
   unsigned int N1;
   unsigned int N2;
   unsigned int N3;
}Element_t;


/* metodos  implementados */
Element_t *new_Element(unsigned int N1,unsigned int N2, unsigned int N3);
void libera_Element(Element_t *T);
unsigned int get_ElementN1(Element_t *T);
unsigned int get_ElementN2(Element_t *T);
unsigned int get_ElementN3(Element_t *T);

#endif
