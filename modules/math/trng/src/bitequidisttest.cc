// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 22:12:32; edited by heiko>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// testing equidistribution of bits of random number generators
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
void bit_equidist(RNG_type &r, long bit, long runs, long n) {
  // r: random number generator
  // bit: bit to test
  // runs: number of Chi-Square tests
  // n: random numbers per test
  vector<double> observ, prob;
  long i, num_fail, mask;
  double Chi2, Chi2P;
  cout.setf(ios::fixed);
  observ.resize(2, 0.0);
  prob.resize(2, 0.5);
  mask=1l<<bit;
  num_fail=0l;
  while (runs>0l) {
    observ[0]=0.0;
    observ[1]=0.0;
    for (i=0l; i<n; ++i)
      if ((r.rand()&mask)>0l)
	++observ[0];
      else
	++observ[1];
    Chi2P=0.0;
    try {
      Chi2=chi_square_test(prob, observ);
      Chi2P=chi_square_prob(Chi2, 1l);
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
void bit_equidist_main(RNG_type &r, long runs, long sub_max, long n) {
  long i, j;
#ifdef LaTeX
  cout << "\\multicol{" << runs+2l <<"}{@{}l@{}}{Generator " << r.name() 
       << "}\\\\" << endl << "\\midrule" << endl;
#else
  cout << "Generator " << r.name() << endl;
#endif
  for (j=1l; j<=sub_max; ++j) {
#ifdef LaTeX
    cout << "\\multicol{" << runs+2l <<"}{@{}l@{}}{Splitting level " << j 
	 << "}\\\\" << endl;
#else
    cout << "Splitting level " << j << endl;
#endif
    for (i=0l; i<31l; ++i) {
#ifdef LaTeX
      cout << "Bit Nr. " << i << " & ";
#else
      cout << "Bit Nr. " << i << '\t';
#endif
      r.reset();
      r.split(j, 0l);
      bit_equidist(r, i, runs, n);
    }
  }
}


int main(int argc, char *argv[]) {
  if (argc!=4) {
    cerr << 
      "Tina's Random Number Generator Library " << TRNG::version() << "\n";
    cerr << 
      "(P) & (C) by Heiko Bauke, Magdeburg 2001\n\n";
    cerr << 
      "Equdistribution test for bits\n";
    cerr << 
      "-----------------------------\n\n";
    cerr << 
      "synopsis:\n";
    cerr << 
      "$ " << argv[0] << " runs sub_max n\n\n";
    cerr <<
      "usefull arguments: runs=5; sub_max=128; n=3000;\n\n"; 
    cerr << 
      "Tests the equdistribution of the bits 0..30 and makes a Chi^2-test.\n\n";
    cerr 
      << "Four further informations see Donald E. Knuth\n";
    cerr << "\"The Art of Computer Programming\", 1981\n";
  } else {
    try {
      long runs, sub_max, n;
      runs=atol(argv[1]);
      sub_max=atol(argv[2]);
      n=atol(argv[3]);
#ifdef LaTeX
      cout << n << " W\\\"urfe je Test\\\\" << endl;
#else
      cout << n << " throws per test" << endl;
#endif

#define FOR_ALL_GENERATORS \
RNG r; bit_equidist_main(r, runs, sub_max, n);
#include "for_all_generators.h"

    }
    catch (...) {
      cerr << "something went wrong" << endl;
    }
  }
  return EXIT_SUCCESS;
}
