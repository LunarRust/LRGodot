// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 22:57:59; edited by heiko>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// permutation test for random number generators
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

using namespace TRNG;
using namespace std;


template<class RNG_type>
void permutation(RNG_type &r, long runs, long n, long m) {
  // r: random number generator
  // runs: number of Chi-Square tests
  // n, m: analyze permutation of n numbers m times
  vector<double> observ, prob;
  vector<long> u;
  long i, j, num_fail, fac, f, q, j_max;
  double Chi2, Chi2P;
  cout.setf(ios::fixed);
  fac=1l;
  for (i=2l; i<=n; ++i)
    fac*=i;
  observ.resize(fac, 0.0);
  prob.resize(fac, 1.0/static_cast<double>(fac));
  u.resize(n, 0l);
  num_fail=0l;
  while (runs>0l) {
    for (i=0l; i<fac; ++i)
      observ[i]=0.0;
    for (i=0l; i<m; ++i) {
      for (j=0l; j<n; ++j)
	u[j]=r.rand();
      q=n;
      f=0l;
      do {
	j_max=0l;
	for (j=1l; j<q; ++j)
	  if (u[j]>u[j_max])
	    j_max=j;
	f=f*q+j_max;
	q--;
	j=u[j_max];
	u[j_max]=u[q];
	u[q]=j;
      } while (q>1);
      ++observ[f];
    }
    Chi2P=0.0;
    try {
      Chi2=chi_square_test(prob, observ);
      Chi2P=chi_square_prob(Chi2, fac-1l);
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
    --runs;
  }
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


template<class RNG_type>
void permutation_main(RNG_type &r, long runs, long sub_max, long n, long m) {
  long j;
#ifdef LaTeX
  cout << "\\multiciol{" << runs+1l <<"}{@{}l@{}}{Generator " << r.name() 
       << "}\\\\" << endl << "\\midrule" << endl;
#else
  cout << "Generator " << r.name() << endl;
#endif
  for (j=1l; j<=sub_max; ++j) {
#ifdef LaTeX
    cout << "\\multiciol{" << runs+2l <<"}{@{}l@{}}{Splitting level " << j 
	 << "}\\\\" << endl;
#else
    cout << "Splitting level " << j << endl;
#endif
    r.reset();
    r.split(j, 0l);
    permutation(r, runs, n, m);
  }
}


int main(int argc, char *argv[]) {
  if (argc!=5) {
    cerr << 
      "Tina's Random Number Generator Library " << TRNG::version() << "\n";
    cerr << 
      "(P) & (C) by Heiko Bauke, Magdeburg 2001\n\n";
    cerr << 
      "Permutation test\n";
    cerr << 
      "----------------\n\n";
    cerr << 
      "synopsis:\n";
    cerr << 
      "$ " << argv[0] << " runs sub_max n m\n\n";
    cerr <<
      "usefull arguments: runs=5; sub_max=128; n=5; m=3000; m>n\n\n";
    cerr << 
      "The Programm examines the relative orderings of m n-tuples\n";
    cerr << 
      "and makes a Chi^2-test.\n\n";
    cerr 
      << "Four further informations see Donald E. Knuth\n";
    cerr << "\"The Art of Computer Programming\", 1981\n";
  } else {
    try {
      long runs, sub_max, n, m;
      runs=atol(argv[1]);
      sub_max=atol(argv[2]);
      n=atol(argv[3]);
      m=atol(argv[4]);
#ifdef LaTeX
      cout << m << " " << n << "-Tupel\\\\" << endl;
#else
      cout << m << " " << n << "-tuples" << endl;
#endif

#define FOR_ALL_GENERATORS \
RNG r; permutation_main(r, runs, sub_max, n, m);
#include "for_all_generators.h"

    }
    catch (...) {
      cerr << "something went wrong" << endl;
    }
  }
  return EXIT_SUCCESS;
}
