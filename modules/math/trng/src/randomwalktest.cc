// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 23:07:59; edited by heiko>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// a random walk test for random number generators
//
// Copyright (C) 2001, 2002 Heiko Bauke
//
// heiko.bauke@physik.uni-magdeburg.de
//
// TRNG is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation. This program
// is distributed WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// ---------------------------------------------------------------------


#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <trng.h>
#include <mpi.h>

using namespace TRNG;
using namespace std;

int rank, size;


template<class RNG_type>
void walk(RNG_type &r, long runs) {
  // r: random number generator
  // runs: number of Chi-Square tests
  const int steps=7;
  const long l=2l*steps+1l;
  long walkers=static_cast<long>(ceil(25.0*pow(8.0, steps)/size));
  vector<double> observ, prob, b;
  long i, j, k, q, x, y, num_fail;
  double Chi2, Chi2P, t;
  cout.setf(ios::fixed);
  r.split(size, rank);
  observ.resize(l*l, 0.0);
  /* calc probability distribution */
  if (rank==0) {
    prob.resize(l*l, 0.0);
    b.resize(l*l, 0.0);
    prob[steps*l+steps]=1.0;
    for (k=0l; k<steps; k++) {
      /* save values for k steps and clear array*/
      for (i=-k; i<=k; i++)
	for (j=-k; j<=k; j++) {
	  b[(steps+i)*l+steps+j]=prob[(steps+i)*l+steps+j];
	  prob[(steps+i)*l+steps+j]=0.0;
	}
      /* calculate probability distribution for k+1 steps */
      /* from the probability distribution for k steps */
      for (i=-k; i<=k; i++)
	for (j=-k; j<=k; j++) {
	  t=b[(steps+i)*l+steps+j]/8.0;
	  prob[(steps+i+1)*l+steps+j  ]+=t;
	  prob[(steps+i+1)*l+steps+j+1]+=t;
	  prob[(steps+i  )*l+steps+j+1]+=t;
	  prob[(steps+i-1)*l+steps+j+1]+=t;
	  prob[(steps+i-1)*l+steps+j  ]+=t;
	  prob[(steps+i-1)*l+steps+j-1]+=t;
	  prob[(steps+i  )*l+steps+j-1]+=t;
	  prob[(steps+i+1)*l+steps+j-1]+=t;
      }
    }
#ifdef field
#ifndef LaTeX
    for (i=0l; i<l; ++i)
      for (j=0l; j<l; ++j)
	if (j<l-1l)
	  cout << setprecision(0) << prob[i*l+j]*walkers*size << '\t';
	else
	  cout << setprecision(0) << prob[i*l+j]*walkers*size << endl;
    cout << endl;
#endif
#endif
    
#ifdef LaTeX
  cout << "\\multiciol{" << runs+1l <<"}{@{}l@{}}{Generator " << r.name() 
       << "}\\\\" << endl;
#else
  cout << "Generator " << r.name() << endl;
#endif    
  }
  num_fail=0l;
  for (k=0l; k<runs; ++k) {
    for (i=0l; i<l*l; ++i)
      observ[i]=0.0;
    for (j=0l; j<walkers; ++j) {
      x=0l;
      y=0l;
      for (i=0l; i<steps; ++i) {
	q=static_cast<long>(8.0*r.uniform());
	switch (q) {
	    case 0l: ++x; break;
	    case 1l: ++x; ++y; break;
	    case 2l: ++y; break;
	    case 3l: --x; ++y; break;
	    case 4l: --x; break;
	    case 5l: --x; --y; break;
	    case 6l: --y; break;
	    case 7l: ++x; --y; break;
	}
      }
      ++observ[(steps+x)*l+steps+y];
    }
    for (i=0l; i<l*l; ++i) {
      t=observ[i];
      MPI::COMM_WORLD.Reduce(&t, &observ[i], 1, MPI::DOUBLE, MPI::SUM, 0);
    }
    if (rank==0) {
#ifdef field
#ifndef LaTeX
      for (i=0l; i<l; ++i)
	for (j=0l; j<l; ++j)
	  if (j<l-1l)
	    cout << setprecision(0) << observ[i*l+j] << '\t';
	  else
	    cout << setprecision(0) << observ[i*l+j] << endl;
#endif
#endif
      Chi2P=0.0;
      try {
	Chi2=chi_square_test(prob, observ);
	Chi2P=chi_square_prob(Chi2, l*l-1l);
      }
      catch (exception &err) {
	cerr << err.what() << endl;
      }
      if (Chi2P<0.01 || 0.99<Chi2P) {
#ifdef LaTeX
	cout << "\\textbf{";
#endif
	++num_fail;
      }
      cout << setfill(' ') << setprecision(3) << Chi2P;
#ifdef LaTeX
      if (Chi2P<0.01 || 0.99<Chi2P)
	cout << "}";
      cout << " & ";
#else
      cout << '\t';
#endif
#ifdef field
#ifndef LaTeX
      cout << endl << endl;
#endif
#endif
    }
  }
  if (rank==0) {
    if (num_fail>2l)
#ifdef LaTeX
      cout << "\\textbf{durchgefallen}";
#else
    cout << "failed";
#endif
    else
      if (num_fail>1l) 
#ifdef LaTeX
	cout << "\\textbf{suspekt}";
#else
        cout << "suspect";
#endif
      else
#ifdef LaTeX
	cout << "o.\\,k.";
#else
        cout << "o.k.";
#endif
#ifdef LaTeX
    cout << "\\\\" << endl;
#else
    cout << endl;
#endif
  }
}


int main(int argc, char *argv[]) {
  if (argc!=2) {
    cerr << 
      "Tina's Random Number Generator Library " << TRNG::version() << "\n";
    cerr << 
      "(P) & (C) by Heiko Bauke, Magdeburg 2001\n\n";
    cerr << 
      "Random walk test in two dimensions V1.0\n";
    cerr << 
      "---------------------------------------\n\n";
    cerr << 
      "synopsis:\n";
    cerr << 
      "$ mpirun -np jobs" << argv[0] << " runs\n\n";
    cerr <<
      "usefull arguments: runs=5\n\n";
    cerr << 
      "The Programm simmulates random walkers on a 2-d-square latice. A random\\n";
    cerr <<
      "walker makes 8 steps in 8 directions with same probability. At least";
    cerr <<
      "25*8^8 random walkers are usesd. The programm stores the last position\n";
    cerr << 
      "and makes a Chi^2-test. Random numbers for the random walks are\n";
    cerr <<
      "generated by leabfrog method.\n";
  } else {
    try {
      long runs;
      runs=atol(argv[1]);
      if (runs<1l) {
	cerr << "invalid argument" << endl;
	exit(EXIT_FAILURE);
      }
      MPI::Init(argc, argv);
      rank=MPI::COMM_WORLD.Get_rank();
      size=MPI::COMM_WORLD.Get_size();
      if (rank==0) {
#ifdef LaTeX
	cout << size << " Streams\\\\" << endl;
#else
	cout << size << " Streams" << endl;
#endif
      }
      
#define FOR_ALL_GENERATORS \
RNG r; walk(r, runs);
#include "for_all_generators.h"

      MPI::Finalize();
    }
    catch (...) {
      cerr << "something went wrong" << endl;
    }
  }
  return EXIT_SUCCESS;
}
