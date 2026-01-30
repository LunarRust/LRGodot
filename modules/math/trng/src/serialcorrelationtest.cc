// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 23:12:37; edited by heiko>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// serial correlation test for random number generators
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
void serialcorrelation(RNG_type &r1, RNG_type &r2, long runs, long n) {
  // r1, r2: random number generator
  // runs: number of tests
  // n: random numbers per test
  long j, num_fail;
  double u, v, c, t1, t2, t3, t4, t5, sigma, mu;
  mu=-1.0/(n-1.0);
  sigma=-2.0*mu*sqrt(n*(n-3.0)/(n+1.0));
  num_fail=0l;
  cout.setf(ios::fixed);
  while (runs>0l) {
    t1=0.0;   
    t2=0.0;   t3=0.0;
    t4=0.0;   t5=0.0;
    for (j=0; j<n; ++j) {
      u=r1.uniform();
      v=r2.uniform();
      t1+=u*v;
      t2+=u;     t3+=v;
      t4+=u*u;   t5+=v*v;
    }
    c=(n*t1-t2*t3)/(sqrt((n*t4-t2*t2)*(n*t5-t3*t3)));
    if (!(mu-sigma<c && c<mu+sigma)) {
      ++num_fail;
#ifdef LaTeX
      cout << "\\textbf{";
#endif
    }
    cout << setfill(' ') << setprecision(3) << c;
#ifdef LaTeX
    if (!(mu-sigma<c && c<mu+sigma))
      cout << "}";
#endif
#ifdef LaTeX
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
void serialcorrelation_main(RNG_type &r1, RNG_type &r2, long sub_max1, 
			    long sub_max2, long runs, long n) {
  long j;
#ifdef LaTeX
  cout << "\\multiciol{" << runs+1l <<"}{@{}l@{}}{Generator " << r1.name() 
       << "}\\\\" << endl << "\\midrule" << endl;
#else
  cout << "Generator " << r1.name() << endl;
#endif
  for (j=2l; j<=sub_max1; ++j) {
#ifdef LaTeX
    cout << "\\multicol{" << runs+1l <<"}{@{}l@{}}{Splitting level " << j 
         << "}\\\\" << endl;
#else
    cout << "Splitting level " << j << " (leapfrog)" <<endl;
#endif
    r1.reset();
    r1.split(j, 0l);
    r2.reset();
    r2.split(j, 1l);
    serialcorrelation(r1, r2, runs, n);
  }
  for (j=1l; j<=sub_max2; ++j) {
#ifdef LaTeX
    cout << "\\multicol{" << runs+1l <<"}{@{}l@{}}{Splitting level " << j 
         << "}\\\\" << endl;
#else
    cout << "Splitting level " << j << " (sequence splitting)" << endl;
#endif
    r1.reset();
    r2.reset();
    r2.jump2(j);
    serialcorrelation(r1, r2, runs, n);
  }
}


int main(int argc, char *argv[]) {
  if (argc!=5) {
    cerr << 
      "Tina's Random Number Generator Library " << TRNG::version() << "\n";
    cerr << 
      "(P) & (C) by Heiko Bauke, Magdeburg 2001\n\n";
    cerr << 
      "Serialcorrelation test\n";
    cerr << 
      "----------------------\n\n";
    cerr << 
      "synopsis:\n";
    cerr << 
      "$ " << argv[0] << " runs sub_max1 sub_max2 n\n\n";
    cerr <<
      "usefull arguments: runs=5; sub_max1=128; sub_max2=32; n=3000;\n\n";
    cerr << 
      "The Programm calculates the correlation between sub sequenzes.\n\n";
    cerr 
      << "Four further informations see Donald E. Knuth\n";
    cerr << "\"The Art of Computer Programming\", 1981\n";
  } else {
    try {
      long runs, sub_max1, sub_max2, n;
      runs=atol(argv[1]);
      sub_max1=atol(argv[2]);
      sub_max2=atol(argv[3]);
      n=atol(argv[4]);
      cout << "length " << n << endl;

#define FOR_ALL_GENERATORS \
RNG ra, rb; serialcorrelation_main(ra, rb, sub_max1, sub_max2, runs, n);
#include "for_all_generators.h"

    }
    catch (...) {
      cerr << "something went wrong" << endl;
    }
  }
  return EXIT_SUCCESS;
}
