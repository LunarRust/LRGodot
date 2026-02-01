// *************************************************************
// 
// Monte-Carlo-pi-Calulation
// 
// *************************************************************


#include <cstdlib>
#include <cmath>
#include <iostream>
#include <trng.h>
#include <mpi.h>

using namespace TRNG;
using namespace std;

int main(int argc, char *argv[]) {

  // number of points in quare
  const long all_samples=1000000l;

  // pseudo random number generator object
  LCG64 r;
  
  // MPI initialisation
  MPI::Init(argc, argv);

  // get rank and number of processes
  int size=MPI::COMM_WORLD.Get_size();
  int rank=MPI::COMM_WORLD.Get_rank();
  
  // split sequence of pseudo random numbers by leapfrog method
  r.split(size, rank);
  
  // no points in the quare
  long in=0l;

  // compute number of points per processor
  long num_samples=all_samples/size;
  // all_samples is not a multiple of size
  if (all_samples%size>rank)
    ++num_samples;

  for (long i=0l; i<num_samples; ++i) {
    double x=r.uniform();
    double y=r.uniform();
    // is point in square
    if (x*x+y*y<=1.0)
      // yes? increment in
      ++in;
  }

  // collect results and print pi
  long in_all;
  MPI::COMM_WORLD.Reduce(&in, &in_all, 1, MPI::LONG, MPI::SUM, 0);
  if (rank==0) {
    double pi=4.0*static_cast<double>(in_all)/static_cast<double>(all_samples);
    cout << "pi = " << pi << endl;
  }

  // quit MPI 
  MPI::Finalize();
  
  return EXIT_SUCCESS;
}
