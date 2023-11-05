#######################################################################
##                      MAKEFILE.OPTIONS                             ##
#######################################################################

#######################################################################
##                         PATH  DEFINITIONS                         ##
#######################################################################

DIR_LOCAL_PATH  = ./
##DIR_INCLS = ./
##-I../Parallel_Vector -I../schedule -I../PCSR -I../CSR -I./files

#######################################################################
##                 COMPILER OPTIONS :  LINUX on PC                   ##
#######################################################################

C           = gcc
OPT          = -Wall -pedantic -O2 -g -lm
OBJS         = msg.o format_file.o CSRmat.o point.o BoundaryEdge.o triangle.o treefunction.o BoundaryCondition.o Mesh.o basis.o NumIntegral.o FEM.o main.o

default : main

main: $(OBJS)
	$(C) $(OPT) -o $@.exe $(OBJS)

clean :
	rm -f *.o *.*~ *~

.SUFFIXES: .c .o
.c.o:
	$(C) $(OPT) $(DIR_INCLS) -c $<
