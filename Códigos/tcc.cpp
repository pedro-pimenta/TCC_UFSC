//                                MFEM Example 0
//
// Compile with: make ex0
//
// Sample runs:  ex0
//               ex0 -m ../data/fichera.mesh
//               ex0 -m ../data/square-disc.mesh -o 2
//
// Description: This example code demonstrates the most basic usage of MFEM to
//              define a simple finite element discretization of the Laplace
//              problem -Delta u = 1 with zero Dirichlet boundary conditions.
//              General 2D/3D mesh files and finite element polynomial degrees
//              can be specified by command line options.

#include "mfem.hpp"
#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;
using namespace mfem;
using namespace chrono;

//const char *mesh_file = "../data/malhas/triang-ret.mesh";
//const char *mesh_file = "../data/malhas/Estru.mesh";
const char *mesh_file = "../data/malhas/unEstru.mesh";
//const char *mesh_file = "../data/custom-mesh-tcc.mesh";

void function_def(Mesh &mesh, double *f, double *U);
void save_matrix(SparseMatrix *A);

int main(int argc, char *argv[])
{
   // 1. Parse command line options.
   int order = 1;

   int refine;
   double h, error;

   OptionsParser args(argc, argv);
   args.AddOption(&mesh_file, "-m", "--mesh", "Mesh file to use.");
   args.AddOption(&order, "-o", "--order", "Finite element polynomial degree");
   args.ParseCheck();

   while(1){
      Mesh mesh(mesh_file);

      cout << "Refine how many times? ";
      cin >> refine;

      for(int i = 0; i < refine; i++){
         // 2. Read the mesh from the given mesh file, and refine once uniformly.
         mesh.UniformRefinement();
      }

      //Inicia contagem do tempo
      auto start_time = high_resolution_clock::now();

      // 3. Define a finite element space on the mesh. Here we use H1 continuous
      //    high-order Lagrange finite elements of the given order.
      H1_FECollection fec(order, mesh.Dimension());
      FiniteElementSpace fespace(&mesh, &fec);
      cout << "Number of unknowns: " << fespace.GetTrueVSize() << endl;

      // 4. Extract the list of all the boundary DOFs. These will be marked as
      //    Dirichlet in order to enforce zero boundary conditions.
      Array<int> boundary_dofs;
      fespace.GetBoundaryTrueDofs(boundary_dofs);

      // 5. Define the solution x as a finite element grid function in fespace. Set
      //    the initial guess to zero, which also sets the boundary conditions.
      GridFunction x(&fespace);
      x = 0.0;

      // 6. Set up the linear form b(.) corresponding to the right-hand side.
      ConstantCoefficient one(1.0);
      GridFunction function(&fespace);

      double *U = (double*)malloc(mesh.GetNV()*sizeof(double));
      double *f = (double*)malloc(mesh.GetNV()*sizeof(double));

      LinearForm b(&fespace);

      // for(int i = 0; i < mesh.GetNV(); i++){
      //    cout << b[i] << endl;
      // }
      
      function_def(mesh, f, U);
      b.AddDomainIntegrator(new DomainLFIntegrator(one));
      b.Assemble();

      for(int i = 0; i < mesh.GetNV(); i++){
         b[i] = b[i]*f[i];
      }

      // 7. Set up the bilinear form a(.,.) corresponding to the -Delta operator.
      BilinearForm a(&fespace);
      a.AddDomainIntegrator(new DiffusionIntegrator);
      a.Assemble();

      // 8. Form the linear system A X = B. This includes eliminating boundary
      //    conditions, applying AMR constraints, and other transformations.
      SparseMatrix A;
      Vector B, X;

      a.FormLinearSystem(boundary_dofs, x, b, A, X, B);

      save_matrix(&A);

      // 9. Solve the system using PCG with symmetric Gauss-Seidel preconditioner.
      GSSmoother M(A);
      //PCG(A, M, B, X, 1, 200, 1e-12, 0.0);
      SLI(A, M, B, X, 1, 20000, 1e-12, 0.0);
      //PCG(A, M, B, X, 1, 200, 1e-12, 0.0); Reference
      //CG(A, B, X, 1, 200, 1e-12, 0.0);

      //Calcula erro
      Vector E;
      E.SetSize(mesh.GetNV());

      for(int i = 0; i < mesh.GetNV(); i++){
         E[i] = X[i] - U[i];
         if(i == 0){
            error = fabs(E[i]);
         }
         if(error < fabs(E[i])){
            error = fabs(E[i]);
         }
      }

      //Para contagem do tempo
      auto end_time = high_resolution_clock::now();
      auto duration = duration_cast<milliseconds>(end_time - start_time);

      cout << "Quantidade de elementos: " << mesh.GetNE() << endl;
      cout << "Erro: " << error << endl;
      cout << "Tempo: " << duration.count() << endl;

      // 10. Recover the solution x as a grid function and save to file. The output
      //     can be viewed using GLVis as follows: "glvis -m mesh.mesh -g sol.gf"
      a.RecoverFEMSolution(X, b, x);
      x.Save("sol.gf");
      mesh.Save("mesh.mesh");
   }

   return 0;
}

void function_def(Mesh &mesh, double *f, double *U)
{
   double *xy_coord;
   double x, y;

   for(int i = 0; i < mesh.GetNV(); i++){
      xy_coord = mesh.GetVertex(i);
      x = xy_coord[0];
      y = xy_coord[1];

      // Laplaciano de u
      f[i] = 2*pow(M_PI,2)*sin(M_PI*x)*sin(M_PI*y);

      // Função exata
      U[i] = sin(M_PI*x)*sin(M_PI*y);
   }

   return;
}

void save_matrix(SparseMatrix *A)
{
   DenseMatrix K = *A->ToDenseMatrix();

   ofstream myfile("matrix.txt");

   int i, j;
   for(i = 0; i < K.Size(); i++){
      for(j = 0; j < K.Size(); j++){
         if(j == K.Size()-1){
            myfile << K.Elem(i, j);
         }
         else{
            myfile << K.Elem(i, j) << ",";
         }
      }
      myfile << "\n";
   }

   myfile.close();

   return;
}