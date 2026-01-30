// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 23:26:32; edited by heiko>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// seral testing for random number generators
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
void serial_test(RNG_type &r, long runs, long n, long num_classes) {
  // r: random number generator
  // bit: bit to test
  // runs: number of Chi-Square tests
  // n: random numbers per test
  // num_classes: number of classes for equidistribution test
  vector<double> observ, prob;
  long i, num_fail;
  double Chi2, Chi2P;
  cout.setf(ios::fixed);
  observ.resize(num_classes*num_classes);
  prob.resize(num_classes*num_classes, 
	      1.0/num_classes/num_classes);
  num_fail=0l;
  while (runs>0l) {
    for (i=0; i<num_classes*num_classes; ++i)
      observ[i]=0.0;
    for (i=0l; i<n; ++i)
      ++observ[static_cast<long>(r.uniform()*num_classes)*num_classes
	      +static_cast<long>(r.uniform()*num_classes)];
    Chi2P=0.0;
    try {
      Chi2=chi_square_test(prob, observ);
      Chi2P=chi_square_prob(Chi2, num_classes*num_classes-1l);
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
void serial_test_main(RNG_type &r, long runs, long sub_max, long n, long num_classes) {
  long j;
#ifdef LaTeX
  cout << "\\multicol{" << runs+1l <<"}{@{}l@{}}{Generator " << r.name() 
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
    r.reset();
    r.split(j, 0l);
    serial_test(r, runs, n, num_classes);
  }
}


int main(int argc, char *argv[]) {
  if (argc!=5) {
    cerr << 
      "Tina's Random Number Generator Library " << TRNG::version() << "\n";
    cerr << 
      "(P) & (C) by Heiko Bauke, Magdeburg 2001\n\n";
    cerr << 
      "Serial test\n";
    cerr << 
      "-----------\n\n";
    cerr << 
      "synopsis:\n";
    cerr << 
      "$ " << argv[0] << " runs sub_max n num_classes\n\n";
    cerr <<
      "usefull arguments: runs=5; sub_max=128; n=5000; num_classes=12; n>num_classes^2\n";
    cerr <<
      "The Programm counts how often pairs (p1, p2) (0<=p_i<num_classes)\n";
    cerr <<  
      "appear and makes a Chi^2-test.\n\n";
    cerr 
      << "Four further informations see Donald E. Knuth\n";
    cerr << "\"The Art of Computer Programming\", 1981\n";
  } else {
    try {
      long runs, sub_max, n, num_classes;
      runs=atol(argv[1]);
      sub_max=atol(argv[2]);
      n=atol(argv[3]);
      num_classes=atol(argv[4]);
#ifdef LaTeX
      cout << num_classes << " Klassen \\\\" << endl;
      cout << n << " W\\\"urfe je Test\\\\" << endl;
#else
      cout << num_classes << " classes" << endl;
      cout << n << " throws per test" << endl;
#endif

#define FOR_ALL_GENERATORS \
RNG r; serial_test_main(r, runs, sub_max, n, num_classes);
#include "for_all_generators.h"

    }
    catch (...) {
      cerr << "something went wrong" << endl;
    }
  }
  return EXIT_SUCCESS;
}
