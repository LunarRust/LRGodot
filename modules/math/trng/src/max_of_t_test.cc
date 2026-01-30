// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 22:53:52; edited by heiko>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// maximum-of-t-test for random number generators
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
void max_of_t(RNG_type &r, long runs, long t, long n, long num_classes) {
  // r: random number generator
  // runs: number of Chi-Square tests
  // t, n: analyze run of t numbers n times
  // num_classes: number of classes for equidistribution test 
  long i, j, num_fail;
  double u, u_max, Chi2, Chi2P;
  vector<double> observ, prob;
  cout.setf(ios::fixed);
  prob.resize(num_classes, 1.0/static_cast<double>(num_classes));
  observ.resize(num_classes, 0.0);
  num_fail=0l;
  while (runs>0l) {
    for (i=0l; i<n; ++i) {
      u_max=0.0;
      for (j=0l; j<t; ++j)
	if ((u=r.uniform())>u_max)
	  u_max=u;
      ++observ[static_cast<long>(num_classes*pow(u_max, static_cast<double>(t)))];
    }
    Chi2P=0.0;
    try {
      Chi2=chi_square_test(prob, observ);
      Chi2P=chi_square_prob(Chi2, num_classes-1l);
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
void max_of_t_main(RNG_type &r, long runs, long sub_max, long t, long n, long num_classes) {
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
    max_of_t(r, runs, t, n, num_classes);
  }
}


int main(int argc, char *argv[]) {
  if (argc!=6) {
    cerr << 
      "Tina's Random Number Generator Library " << TRNG::version() << "\n";
    cerr << 
      "(P) & (C) by Heiko Bauke, Magdeburg 2001\n\n";
    cerr << 
      "Maximum-of-t test\n";
    cerr << 
      "-----------------\n\n";
    cerr << 
      "synopsis:\n";
    cerr << 
      "$ " << argv[0] << " runs sub_max t n num_classes\n\n";
    cerr <<
      "usefull arguments: runs=5; sub_max=128; t=8; n=600; num_classes=25;\n\n";
    cerr << 
      "The programm selects the maximum M(t) of a t-tuple and tests if the\n";
    cerr <<
      "M(t)^t are equidistributed. num_classes classes are used for the\n";
    cerr << 
      " equidistribution test. The test is n times applied.\n\n";
    cerr 
      << "Four further informations see Donald E. Knuth\n";
    cerr << "\"The Art of Computer Programming\", 1981\n";
  } else {
    try {
      long runs, sub_max, t, n, num_classes;
      runs=atol(argv[1]);
      sub_max=atol(argv[2]);
      t=atol(argv[3]);
      n=atol(argv[4]);
      num_classes=atol(argv[5]);
#ifdef LaTeX
      cout << n << " " << t << "-Tupel \\\\" << endl;
      cout << num_classes << " Klassen \\\\" << endl;
#else
      cout << n << " " << t << "-tuples" << endl;
      cout << num_classes << " classes" << endl;
#endif

#define FOR_ALL_GENERATORS \
RNG r; max_of_t_main(r, runs, sub_max, t, n, num_classes);
#include "for_all_generators.h"

    }
    catch (...) {
      cerr << "something went wrong" << endl;
    }
  }
  return EXIT_SUCCESS;
}
